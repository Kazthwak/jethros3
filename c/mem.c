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
	max_mem += ONE_MB; //Now adjusted for the ignored first MB of memory
	first_mem_hole = max_mem;
	for(uint16_t i= 0; i < 1024; i++){
		page_tables[i] = 0;
	}
	page_tables[768] = &page_table_kernel_1;
	page_tables[769] = &page_table_kernel_2;
	page_tables[770] = &page_table_kernel_3;
	init_phys_pages();
	next_free_kernel_mem = (uint32_t)end_of_used_ram;
	//align it to the next page boundary
	next_free_kernel_mem >>= 12;
	next_free_kernel_mem++;
	next_free_kernel_mem <<= 12;
}

void init_mem_late(){
	//create new page tables
	for(uint16_t i = 771; i < 1024; i++){
		//allocate a page table
		uint32_t* page_table = (uint32_t*)kmalloc_permanant_page();
		//fill with 0
		memset((uint32_t)page_table, 0x0, page_size);
		//put it in the page directory and the page table list
		page_tables[i] = page_table;
		page_directory[i] = (uint32_t)page_table | 3;
	}
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
	//check if no page table present
	if(page_table == 0){
		print_string("No page table present for this virtual address.\n");
		hang();
	}
	page_table[table_entry] = phys_addr | 0b11;
}

bool alloc_and_map_page(uint32_t virt_addr){
	uint32_t phys_addr = (uint32_t)alloc_phys_page(0x0);
	if(phys_addr == 0xffffffff){
		return(false);
	}
	map_page(phys_addr, virt_addr);
	return(true);	
}

//virtual address space todos
//virtual address kernel memory manager
//load programs by mapping pages and putting program in
//leave the rest below 2gb as unmapped pages and handle page "faults"
//swap out programs by changing the page directory. leave program page tables in kernel memory, and keep
//pointers to the first 2-3gb (decide)

//kernel virtual manager functions

//this, could be tricky
//I need to maintain a map of memory, but in a granularity of 1 byte
//I could use a linked list, but that would necessitate random pages being mapped
//I think a better solution (for now) is to allocate memory that cannot be de-allocated
//I can use a heap to allocate small structures that will be de-allocated
//i will also need a function to allocate the next page, which will waste up to 4096B of memory. I see now why a more
//intelligent memory manager is so useful (there is a bug that that means it can waste 4KB rather than 4KB-1)
//todo
//wirk out which pages are crossed by the allocation
//allocate those which are de-allocated
//instead, I will make a page fault allocate the memory
//profit?

//returns a pointer to the next {length} bytes after the last used piece of kernel memory
uint32_t kmalloc_permanant(uint32_t length){
	uint32_t start = next_free_kernel_mem;
	uint32_t end = start+length-1; //last used byte
	next_free_kernel_mem = end+1; //first byte after, the next free byte
	return(start);
}

uint32_t kmalloc_permanant_page(){
	uint32_t start = next_free_kernel_mem;
	start >>= 12;
	start++;
	start <<=12;
	//start now points to the next page aligned value;
	next_free_kernel_mem = start+page_size;
	return(start);
}
