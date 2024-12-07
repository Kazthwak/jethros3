#include "../kernel.h"

#define page_address(page_n) (page_n*page_size)

//after initialising, the first page table (first 4MB) is mapped to 0xc0000000
//next step is to find the video memory, and map it
//functions needed:
//


void mem_init(){
	end_of_used_ram = &_kernel_end;
	beg_of_used_ram = (void*)(((uint32_t)&_kernel_start)+PAGING_OFFSET);
	uint32_t max_mem = stateinfo.mem_upper;
	max_mem *= 1024; //now contains the size of upper memory in bytes
	max_mem += 1024*1024; //Now adjusted for the ignored first MB of memory
	first_mem_hole = max_mem;
	for(uint16_t i= 0; i < 1024; i++){
		page_tables[i] = 0;
	}
	page_tables[768] = &page_table_kernel_1;
	page_tables[769] = &page_table_kernel_2;
	init_phys_pages();
}

//pages on the boundaries of usability may be flagged as unusable when they are usable (off by one error)
//I am being conservative to ensure no memory-mapped memory is used
uint8_t init_page_valid(uint32_t mem_addr){
	//end of page is after start of OS and start of page is before end of OS
	//means page includes OS image. May malfunction if the OS image is less than 1 page in size
	if((mem_addr+page_size) >= (uint32_t)beg_of_used_ram && mem_addr <= (uint32_t)end_of_used_ram){
		return(0);
	}
	//if the address is less than 1MB
	if(mem_addr < 0x400*0x400){
		//before the EBDA. this is the free memory, and the IVT and BDA which are free to be overwritten
		if(mem_addr < 0x80000){
			return(1);
		}
		//memory is after the start of the EBDA, which means it is a part of the EBDA, or is otherwise reserved
		else{
			return(0);
		}
	}
	//address is in upper memory and not part of OS
	else{
		//end of page is before memory hole
		if((mem_addr+page_size) < first_mem_hole){
			return(1);
		}
		//page is past first memory hole (may be usable but to start with I am marking as unusable)
		else{
			return(0);
		}
	}

}

//1 is free
//0 is used
void init_phys_pages(){
	for(uint32_t i = 0; i < num_pages; i++){
		//i is page num
		uint32_t i_addr = page_address(i);
		uint8_t page_val = init_page_valid(i_addr);
		if(i%32 == 0){phys_page_state[i/32] = 0;}
		phys_page_state[i/32] |= page_val<<(i%32);
	}
//I have no idea what this is supposed to achieve. Why mark the last pages as free for no reason?
//	phys_page_state[(num_page_entries/32)-1] = 0xffffffff;
}

void* alloc_phys_page(uint32_t start_addr){
	for(uint32_t i = (start_addr/32/page_size); i < num_page_entries; i++){
		/*
		text_serial();
		string_serial("\nPage_state : "); hexdword(phys_page_state[i]);
		text_screen();
		*/
		if(phys_page_state[i] > 0){
			uint32_t state = phys_page_state[i];
			uint32_t pow2 = 1;
			for(uint8_t j = 0; state > 0; j++){
				if((state&0x1) == 1){
					phys_page_state[i] &= (pow2^0xffffffff);
					return((void*)(page_size*((i*32)+j)));
				}
				pow2 <<= 1;
				state >>= 1;
			}
			return((void*)0xffffffff);
		}
	}
	return((void*)0xffffffff);
}

uint32_t get_phys_address(uint32_t address){
	uint16_t dir_entry;
	uint16_t table_entry;
	uint32_t offset;
	//most significant 10 bits
	dir_entry = address>>22;
	//next 10 most significant bits
	table_entry = (address>>12)&0b1111111111;
	//least significant 12 bits
	offset = address&0b111111111111;
	uint32_t dir_entry_data = page_directory[dir_entry];
	if((dir_entry_data&0b1) == 0){return(0xffffffff);}
	uint32_t* page_table = (uint32_t*)page_tables[dir_entry];
	uint32_t table_entry_data = page_table[table_entry];
	if((table_entry_data&1) == 0){return(0xffffffff);}
	uint32_t tmp = (table_entry_data>>12)<<12;
	return(tmp+offset);
}

void map_page(uint32_t phys_addr, uint32_t virt_addr){
	uint16_t dir_entry;
	uint16_t table_entry;
	//most significant 10 bits
	dir_entry = virt_addr>>22;
	//next 10 most significant bits
	table_entry = (virt_addr>>12)&0b1111111111;
	uint32_t* page_table = (uint32_t*)page_tables[dir_entry];
	page_table[table_entry] = phys_addr | 0b11;
}