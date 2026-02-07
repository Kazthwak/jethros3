#include "../kernel.h"

#define page_address(page_n) (page_n*page_size)

//after initialising, the first page table (first 4MB) is mapped to 0xc0000000
//next step is to find the video memory, and map it
//functions needed:


uint32_t page_table_0[1024] __attribute__((aligned(4096))) = {0};

void* phys_mem_pokage;
uint32_t phys_mem_pokage_last_base = 0xffffffff;

void mem_init(){
	end_of_used_ram = &_kernel_end;
	beg_of_used_ram = (void*)(((uint32_t)&_kernel_start)+PAGING_OFFSET);
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
	phys_mem_pokage = (void*)kmalloc_permanent_page();
}

void init_mem_late(){
	//create new page tables
	for(uint16_t i = 771; i < 1024; i++){
		//allocate a page table
		uint32_t* page_table = (uint32_t*)kmalloc_permanent_page();
		//fill with 0
		memset(page_table, 0x0, page_size);
		//put it in the page directory and the page table list
		page_tables[i] = page_table;
		page_directory[i] = get_phys_address((uint32_t)page_table) | 3;
	}
	
	page_tables[0] = page_table_0;	
	page_directory[0] = get_phys_address((uint32_t)page_table_0) | 3;
	
	k_heap_allocator_init();
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
	if(get_phys_address(virt_addr) != 0xffffffff){
		return(true);
	}
	uint32_t phys_addr = (uint32_t)alloc_phys_page(0x0);
	if(phys_addr == 0xffffffff){
		return(false);
	}
	map_page(phys_addr, virt_addr);
	return(true);	
}

void ready_phys_address(uint32_t address){
	uint32_t page_base = (address/page_size)*page_size;
	if(page_base == phys_mem_pokage_last_base && 0){
		return;
	}
	phys_mem_pokage_last_base = page_base;
	map_page(page_base, (uint32_t)phys_mem_pokage);
}

uint32_t peak_phys_address(uint32_t address){
	print_string("THIS DOES NOT WORK");
	ready_phys_address(address);
	uint32_t* addr = (uint32_t*)((address%page_size) + (uint32_t)phys_mem_pokage);
	return(*addr);
}

void poke_phys_address(uint32_t address, uint32_t value){
	print_string("THIS DOES NOT WORK");
	ready_phys_address(address);
	uint32_t* addr = (uint32_t*)((address%page_size) + (uint32_t)phys_mem_pokage);
	*addr = value;
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


//returns a pointer to the next {length} bytes after the last used piece of kernel memory
uint32_t kmalloc_permanent(uint32_t length){
	uint32_t start = next_free_kernel_mem;
	uint32_t end = start+length-1; //last used byte
	next_free_kernel_mem = end+1; //first byte after, the next free byte
	return(start);
}

uint32_t kmalloc_permanent_page(){
	uint32_t start = next_free_kernel_mem;
	if(start != (start&0xfffff000)){
	start >>= 12;
	start++;
	start <<=12;
	}
	//start now points to the next page aligned value;
	next_free_kernel_mem = start+page_size;
	return(start);
}

//Old memory allocator. I am completely rewriting in a new file
/*
//heap work:
//some large un-deallocateable section of memory. extendable?
//maybe 32MB - use a define to make it easy to change
//keep the whole thing lazy loaded at all times

//slab alloc - find the first element of the linked list with length greater than LENGTH+4, shorten the length of the list
//(possibly removing it), and return a pointer to the last LENGTH bytes of that section of memory. Put a uint32_t before
//the memory recording its length (including itself)
//if no memory is available, attempt to amalgamate neighbouring sections of the slab
//slab free - decrement the pointer by 4, and then read length (including the uint32_t before the alloced memory)
//add a link in the list there (keeping it sorted)

//for allocating smaller amounts of memory
uint32_t kmalloc_slab(uint32_t length){
	length +=4;
	uint32_t prev_pointer = 0x0;
	uint32_t cur_pointer = first_free_slab;
	while(true){
		struct free_mem_link* link = (struct free_mem_link*)cur_pointer;
		if(link->length >= length){break;}
		prev_pointer = cur_pointer;
		cur_pointer = link->next_free_pointer;
		if(cur_pointer == 0x0){
			//dans the future, this should defragment the list, then call itself recursively, somehow making sure it
			//doesn't loop infinitely?
			return(0x0);
		}
	}
	struct free_mem_link* link = (struct free_mem_link*)cur_pointer;
	uint32_t final_address = 0x0;
	if((link->length)-length > 8){
		//big enough to shorten, but leave in place
		final_address = cur_pointer+(link->length)-length;
		link->length -=length;
	}
	else{
		//too small to shorten
		//allocate more memory (the size of the block)
		length = link->length;
		final_address = cur_pointer;
		//tremendous jank to make up for the changing of the previous link (check if 0 to update initial pointer)
		if(prev_pointer == 0x0){
			first_free_slab = link->next_free_pointer;
		}
		else{
			//update the previous link
			struct free_mem_link* prev_link = (struct free_mem_link*)prev_pointer;
			prev_link->next_free_pointer = link->next_free_pointer;
		}
	}
	//first_free_slab pointer. find the last (updated) length bytes, put a length at the begining, then return it+4
	if(final_address == 0){
		print_string("Slab allocation has reached an invalid state with no memory to return (with no error condition)");
		hang();
	}
	uint32_t* length_free_pointer = (uint32_t*)final_address;
	*length_free_pointer = length;
	return(final_address+4);
}

void kfree_slab(uint32_t memory){
	uint32_t address = memory-4;
	uint32_t length = *(uint32_t*) address;
	if(address+length > slab_end){
		print_string("MEMORY EXCEEDS END OF SLAB");
		hang();
	}
	if(address < slab_start){
		print_string("MEMORY EXCEEDS BEGINNING OF SLAB");
		hang();
	}
	struct free_mem_link* new_link = (struct free_mem_link*)address;
	new_link->length = length;
	//step through linked list
	
	struct free_mem_link* prev_link = 0x0;
	struct free_mem_link* cur_link = (struct free_mem_link*)first_free_slab;
	
	while(true){
		//current link is after the one trying to be freed
		if(cur_link > new_link){
			break;
		}
		//update the previous link
		prev_link = cur_link;
		//step through the linked list;
		if((cur_link->next_free_pointer) == 0x0){
			//jank. This means that we have reached the last link, so this link is the one to use
			cur_link->next_free_pointer = (uint32_t)new_link;
			new_link->next_free_pointer = 0x0;
			return;
		}
		cur_link = (struct free_mem_link*)cur_link->next_free_pointer;
	}
	//change the previous link to point to this new one
	prev_link->next_free_pointer = (uint32_t)new_link;
	//update the new link to point to the next link
	new_link->next_free_pointer = (uint32_t)cur_link;
}//*/
