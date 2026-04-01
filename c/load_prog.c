//unfortunately prog is short for programme, not progressive rock
#include "../kernel.h"

#define ARBITRARY_SIZE_LIMIT (8*0x400) //(8KB)

//TODO:
//This code makes a new task and puts it in the scheduler queue thing
//Then a function can be called to switch to it

struct elf_header{
	uint32_t magic; //this is actuall a string, but who cares?
	uint8_t bits;
	uint8_t endianess;
	uint8_t padding_or_garbage[10];
	uint16_t type;
	uint16_t machine_type;
	uint32_t elf_ver;
	uint32_t entry;
	uint32_t program_header_off;
	uint32_t section_header_off;
	uint32_t flags;
	uint16_t elf_size;
	uint16_t program_head_ent_size;
	uint16_t num_program_head_ents;
	uint16_t section_head_ent_size;
	uint16_t num_section_head_ents;
	uint16_t section_header_string_index;
}__attribute__((packed));

struct elf_program_ent{
	uint32_t type;
	uint32_t file_offset;
	uint32_t target_address;
	uint32_t pad_phys_address;
	uint32_t size;
	uint32_t mem_size;
	uint32_t flags;
	uint32_t align;
}__attribute__((packed));


char tmp_stack[512];

//TODO FIX this so that it has safety checks. Currently an elf could corrupt the whole computer by being weird
int load_program_and_execute(char* name){
	void* prog_ptr = fl_fopen(name, "r");
	if(prog_ptr == 0){return(0);}
	fl_fseek(prog_ptr, 0, SEEK_END);
	uint32_t size = fl_ftell(prog_ptr);
	if(size > ARBITRARY_SIZE_LIMIT){
		print_string("FILE TOO LARGE");
		return(0);
	}
	//read header
	fl_fseek(prog_ptr, 0, SEEK_SET);
	uint8_t header[512];
	fl_fread(header, 1, 512, prog_ptr);
	struct elf_header* prog_header = (struct elf_header*)header;
	if(prog_header->magic != 0x464C457F || prog_header->bits != 1 || prog_header->endianess != 1){
		print_string("INVALID HEADER");
		return(0);
	}
	if(prog_header->type != 2){
		print_string("NOT AN EXECUTABLE FILE");
		return(0);
	}
	if(prog_header->machine_type != 3){
		print_string("NOT AN INTEL EXECUTABLE");
		return(0);
	}
	if(prog_header->program_head_ent_size < sizeof(struct elf_program_ent)){
		print_string("PROGRAM ENTRIES SMALLER THAN STRUCT?");
		return(0);
	}
	uint32_t program_header_size = (prog_header->program_head_ent_size)*(prog_header->num_program_head_ents);
	if(program_header_size + prog_header->program_header_off > 512){
		hexword(program_header_size + prog_header->program_header_off);
		print_string(" IS LARGER THAN LOADED PORTION OF FILE");
		return(0);
	}
	vector* pages_used = new_vector();
	for(uint16_t i = 0; i < prog_header->num_program_head_ents; i++){
		//chatgpt informs me that this is difficult to follow. I can't imagine this being a problem in the future?'
		struct elf_program_ent elf_prog_head_ent = *(struct elf_program_ent*)(i*(prog_header->program_head_ent_size) + prog_header->program_header_off + header);
		if(elf_prog_head_ent.type ==0 || elf_prog_head_ent.type >= 4){continue;}
		else if(elf_prog_head_ent.type != 1){
			print_string("INCOMPATABLE ELF SECTION TYPE");
			return(0);
		}
		//is a 1 == load
		hexword(elf_prog_head_ent.file_offset);newline();hexdword(elf_prog_head_ent.type);newline();newline();
		uint32_t aligned_loc = (elf_prog_head_ent.target_address)&0xfffff000;
		uint32_t bonus_jank = elf_prog_head_ent.target_address-aligned_loc;
		uint16_t pages_needed = (elf_prog_head_ent.mem_size + 0xfff + bonus_jank)/0x1000;
		uint32_t prev_entries = pages_used->length;
		//make the vector bigger
		pages_used = resize_vector(pages_used, pages_needed*2 + prev_entries);
		for(uint16_t j = 0; j < pages_needed; j++){
			//work out the address
			uint32_t page_base = aligned_loc + j*0x1000;
			//record at the even location the virtual address
			pages_used->data[2*j+prev_entries] = page_base;
			//allocate a physical page
			alloc_and_map_page(page_base);
			//record at the odd location the physical address for swapping back in (when I implement that)
			pages_used->data[2*j+1+prev_entries] = get_phys_address(page_base);
		}
		//addresses set up, now load
		//clear the memory
		memset((void*)elf_prog_head_ent.target_address, 0x0, elf_prog_head_ent.mem_size);
		//load into memory
		fl_fseek(prog_ptr, elf_prog_head_ent.file_offset, SEEK_SET);
		uint32_t tmp = fl_fread((void*)elf_prog_head_ent.target_address, 1, elf_prog_head_ent.size, prog_ptr);
		if(tmp != elf_prog_head_ent.size){
			//this is an inside joke
			print_string("THROW SCREEN OUT OF WINDOW (File read error)");
		}
	}
	//backup pages_used into a struct somewhere - currently it is just a memory leak
	//setup stack
	//setup kernel stack
	// goto user mode
	/*
	clear_keyboard_buffer();
	while(!is_key_waiting());
	clear_keyboard_buffer();
	*/
	// intoff();
	update_tss_stack_ptr(0x10, tmp_stack);
	iret_to_address(prog_header->entry, (void*)((uint32_t)header) + 511);
	return(1);
}