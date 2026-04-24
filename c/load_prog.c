//unfortunately prog is short for programme, not progressive rock
#include "../kernel.h"

#define ARBITRARY_SIZE_LIMIT (8*0x400) //(8KB)
//size in 4kB pages. 16= 64k
#define LOADED_PROG_STACK_SIZE 16
//size in bytes
#define LOADED_PROG_KERN_STACK_SIZE 2*0x1000 //= 8k

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


#define CHECK_VECTOR(vector) if(vector == 0){print_string("ERROR DURING VECTOR RESIZING IN PROGRAM ALLOCATION"); goto cleanup;}

void fork(struct regs* r){
	uint16_t i = 0;
	for(; i < tasks->length; i++){
		if(((struct task_data*)tasks->data[i])->id == current_task){
			break;
		}
		if(i == tasks->length-1){
			print_string("ERROR TASK NOT FOUND\n");
			hang();
		}
	}
	
	uint8_t tmp_data[4*0x400]; //4kB = one page
	vector* pages_used = 0;
	struct task_data* task_info = 0;
	struct regs* machine_state = 0;
	uint8_t* kernel_stack = 0;
	
	//i is task index
	task_info = kmalloc(sizeof(struct task_data));
	if(task_info == 0){
		print_string("TASK INFO ALLOCATION FAILED FORK\n");
		goto cleanup;
	}
	machine_state = kmalloc(sizeof(struct regs));
	if(machine_state == 0){
		print_string("FAILIURE IN ALLOCATING REGISTER STRUCTURE FORK\n");
		goto cleanup;
	}
	memcpy(machine_state, r, sizeof(struct regs));
	machine_state -> ebx = 0;
	
	pages_used = new_vector();
	resize_vector(pages_used, ((struct task_data*)tasks->data[i])->pages_used->length);
	if(pages_used == 0){
		print_string("ERROR MAKING PAGES_USED VECTOR\n");
		goto cleanup;
	}
	//copy data from old task to new one
	for(uint16_t j = 0; j < pages_used->length; j += 2){
		//copy the virtual address
		pages_used->data[j] = ((struct task_data*)tasks->data[i])->pages_used->data[j];
		//allocate a new physical page for it
		pages_used->data[j+1] = (uint32_t)alloc_phys_page(0);
		//check that allocation was succesful
		if(pages_used->data[j+1] == 0xffffffff){
			print_string("FAILED TO ALLOCATE PHYS PAGE\n");
			goto cleanup;
		}
		//copy the data in the page to a temporary area
		memcpy(tmp_data, (void*)pages_used->data[j], sizeof(tmp_data));
		//map the new physical address in
		unmap_page(pages_used->data[j]);
		map_page(pages_used->data[j+1], pages_used->data[j]);
		//copy it back to the new physical page at the same virtual address
		memcpy((void*)pages_used->data[j], tmp_data, sizeof(tmp_data));
		//revert the mapping so that the task can be resumed
		unmap_page(pages_used->data[j]);
		map_page(((struct task_data*)tasks->data[i])->pages_used->data[j+1], pages_used->data[j]);		
	}
	
	task_info->pages_used = pages_used;
	task_info->saved_regs = machine_state; 
	kernel_stack = kmalloc(LOADED_PROG_KERN_STACK_SIZE);
	if(kernel_stack == 0){
		print_string("COULDN'T ALLOCATE KERNEL STACK");
		goto cleanup;
	}
	task_info->kernel_stack_pointer = (uint32_t)kernel_stack + LOADED_PROG_KERN_STACK_SIZE;
	task_info->id = find_free_task_id();
	if(task_info->id == 0){
		print_string("NO FREE TASK IDs FOUND\n");
		goto cleanup;
	}
	uint16_t tasks_len = tasks->length;
	vector* tmp3 = tasks->resize(tasks, tasks_len + 1);
	if(tmp3 == 0){
		print_string("TASKS VECTOR RESIZING FAILED");
		goto cleanup;
	}
	tasks = tmp3;
	
	
	//data now owned by sheduler. Under no circumstances go to cleaunp because then it would free an in use pointer
	tasks->data[tasks_len] = (uint32_t)task_info;
	
	r->eax = 0;
	return;
	
	cleanup:
	print_string("CLEANUP REACHED\n");
	if(pages_used != 0){
		for(uint16_t j = 0; j < pages_used->length; j+=2){
			dealloc_phys_page(pages_used->data[j+1]);
		}
		destroy_vector(pages_used);
	}
	if(task_info != 0){
		kfree(task_info);
	}
	if(machine_state != 0){
		kfree(machine_state);
	}
	if(kernel_stack != 0){
		kfree(kernel_stack);
	}
	r->eax = 4;
	return;
}

void destroy_process(uint16_t id){
	uint16_t i = 0;
	for(; i < tasks->length; i++){
		if(((struct task_data*)tasks->data[i])->id == id){
			break;
		}
		if(i == tasks->length-1){
			print_string("ERROR TASK NOT FOUND\n");
			hang();
		}
	}
	//i is task index
	struct task_data* dying_task = (struct task_data*)tasks->data[i];
	tasks->data[i] = tasks->data[tasks->length-1];
	tasks->resize(tasks, tasks->length-1);
	kfree((void*)dying_task->saved_regs);
	kfree((void*)dying_task->kernel_stack_pointer);
	vector* pages_used = dying_task->pages_used;
	
	for(uint16_t j = 1; j < pages_used->length; j+= 2){
		dealloc_phys_page(pages_used->data[j]);
	}
	kfree(pages_used);
}

//TODO FIX this so that it has safety checks. Currently an elf could corrupt the whole computer by being weird
int load_program(char* name){
	//Declarations
	void* prog_ptr = fl_fopen(name, "r");
	vector* pages_used = 0;
	struct task_data* task_info = 0;
	struct regs* machine_state = 0;
	uint8_t* kernel_stack = 0;
	
	
	if(prog_ptr == 0){
		print_string("FILE NOT FOUND \n");
		goto cleanup;
	}
	fl_fseek(prog_ptr, 0, SEEK_END);
	uint32_t size = fl_ftell(prog_ptr);
	print_string("FILE SIZE: ");
	hexdword(size);
	print_string(" BYTES\n");
	if(size > ARBITRARY_SIZE_LIMIT && 0){
		print_string("FILE TOO LARGE");
		goto cleanup;
	}
	//read header
	fl_fseek(prog_ptr, 0, SEEK_SET);
	uint8_t header[512];
	fl_fread(header, 1, 512, prog_ptr);
	struct elf_header* prog_header = (struct elf_header*)header;
	if(prog_header->magic != 0x464C457F || prog_header->bits != 1 || prog_header->endianess != 1){
		print_string("INVALID HEADER");
		goto cleanup;
	}
	if(prog_header->type != 2){
		print_string("NOT AN EXECUTABLE FILE");
		goto cleanup;
	}
	if(prog_header->machine_type != 3){
		print_string("NOT AN INTEL EXECUTABLE");
		goto cleanup;
	}
	if(prog_header->program_head_ent_size < sizeof(struct elf_program_ent)){
		print_string("PROGRAM ENTRIES SMALLER THAN STRUCT?");
		goto cleanup;
	}
	uint32_t program_header_size = (prog_header->program_head_ent_size)*(prog_header->num_program_head_ents);
	if(program_header_size + prog_header->program_header_off > 512){
		//hexword(program_header_size + prog_header->program_header_off);
		print_string(" IS LARGER THAN LOADED PORTION OF FILE");
		goto cleanup;
	}
	pages_used = new_vector();
	if(pages_used == 0){
		print_string("ERROR IN CREATING PAGE TRACKING VECTOR");
		goto cleanup;
	}
	//ERROR IS OCCURING HERE SOMEWHERE
	for(uint16_t i = 0; i < prog_header->num_program_head_ents; i++){
		//chatgpt informs me that this is difficult to follow. I can't imagine this being a problem in the future?'
		struct elf_program_ent elf_prog_head_ent = *(struct elf_program_ent*)(i*(prog_header->program_head_ent_size) + prog_header->program_header_off + header);
		if(elf_prog_head_ent.type ==0 || elf_prog_head_ent.type >= 4){continue;}
		else if(elf_prog_head_ent.type != 1){
			print_string("INCOMPATABLE ELF SECTION TYPE");
			goto cleanup;
		}
		//is a 1 == load
		//debug info
		//hexword(elf_prog_head_ent.file_offset);newline();hexdword(elf_prog_head_ent.type);newline();newline();
		uint32_t aligned_loc = (elf_prog_head_ent.target_address)&0xfffff000;
		uint32_t bonus_jank = elf_prog_head_ent.target_address-aligned_loc;
		uint16_t pages_needed = (elf_prog_head_ent.mem_size + 0xfff + bonus_jank)/0x1000;
		uint32_t prev_entries = pages_used->length;
		//make the vector bigger
		vector* tmp4 = resize_vector(pages_used, pages_needed*2 + prev_entries);
		CHECK_VECTOR(tmp4)
		pages_used = tmp4;
		for(uint16_t j = 0; j < pages_needed; j++){
			//work out the address
			uint32_t page_base = aligned_loc + j*0x1000;
			//record at the even location the virtual address
			pages_used->data[2*j+prev_entries] = page_base;
			//allocate a physical page
			//first check if already allocated:
			if(get_phys_address(page_base) != 0xffffffff){
				//if so deallocate it
				unmap_page(page_base);
			}
			if(alloc_and_map_page(page_base) == false){
				print_string("OUT OF FREE PAGES FOR PROGRAM"); // ERROR OCCURS HERE
				goto cleanup;
			}
			//record at the odd location the physical address for swapping back in (when I implement that)
			pages_used->data[2*j+1+prev_entries] = get_phys_address(page_base);
		}
		//flush the page cache to avoid bugs
		page_reload();
		//addresses set up, now load
		//clear the memory
		memset((void*)elf_prog_head_ent.target_address, 0x0, elf_prog_head_ent.mem_size);
		//load into memory
		fl_fseek(prog_ptr, elf_prog_head_ent.file_offset, SEEK_SET);
		uint32_t tmp1 = fl_fread((void*)elf_prog_head_ent.target_address, 1, elf_prog_head_ent.size, prog_ptr);
		if(tmp1 != elf_prog_head_ent.size){
			//this is an inside joke
			print_string("THROW SCREEN OUT OF WINDOW (File read error)");
		}
	}
	print_string("GOT TO HERE2");hang();
	//backup pages_used into a struct somewhere - currently it is just a memory leak
	//setup stack
	uint32_t prev_entries = pages_used->length;
	vector* tmp2 = resize_vector(pages_used, LOADED_PROG_STACK_SIZE*2 + prev_entries);
	CHECK_VECTOR(tmp2)
	pages_used = tmp2;
	for(uint16_t i = 0; i < LOADED_PROG_STACK_SIZE; i++){
		uint32_t page_base = 0xc0000000-((i+1)*0x1000);
		pages_used->data[2*i+prev_entries] = page_base;
		//first check if already allocated:
		if(get_phys_address(page_base) != 0xffffffff){
			//if so deallocate it
			unmap_page(page_base);
		}
		if(alloc_and_map_page(page_base) == false){
			print_string("OUT OF FREE PAGES FOR STACK");
			goto cleanup;
		}
		pages_used->data[2*i+1+prev_entries] = get_phys_address(page_base);
	}
	//setup kernel stack
	// goto user mode
	/*
	 * clear_keyboard_buffer();
	 * while(!is_key_waiting());
	 * clear_keyboard_buffer();
	 */
	// intoff();
	
	task_info = kmalloc(sizeof(struct task_data));
	if(task_info == 0){
		print_string("TASK INFO ALLOCATION FAILED");
		goto cleanup;
	}
	machine_state = kmalloc(sizeof(struct regs));
	if(machine_state == 0){
		print_string("FAILIURE IN ALLOCATING REGISTER STRUCTURE");
		goto cleanup;
	}
	memset(machine_state, 0x00, sizeof(struct regs));
	machine_state->gs = (4 * 8) | 3;
	machine_state->fs = (4 * 8) | 3;
	machine_state->es = (4 * 8) | 3;
	machine_state->ds = (4 * 8) | 3;
	machine_state->ss = (4 * 8) | 3;
	machine_state->useresp = 0xbffffff0;
	machine_state->eflags = 0x202;
	machine_state->eip = prog_header->entry;
	machine_state->cs = (3 * 8) | 3;
	
	task_info->pages_used = pages_used;
	task_info->saved_regs = machine_state; 
	kernel_stack = kmalloc(LOADED_PROG_KERN_STACK_SIZE);
	if(kernel_stack == 0){
		print_string("COULDN'T ALLOCATE KERNEL STACK");
		goto cleanup;
	}
	task_info->kernel_stack_pointer = (uint32_t)kernel_stack + LOADED_PROG_KERN_STACK_SIZE;
	task_info->id = find_free_task_id();
	if(task_info->id == 0){
		print_string("NO FREE TASK IDs FOUND\n");
		goto cleanup;
	}
	uint16_t tasks_len = tasks->length;
	vector* tmp3 = tasks->resize(tasks, tasks_len + 1);
	if(tmp3 == 0){
		print_string("TASKS VECTOR RESIZING FAILED");
		goto cleanup;
	}
	tasks = tmp3;
	
	
	//data now owned by sheduler. Under no circumstances go to cleaunp because then it would free an in use pointer
	tasks->data[tasks_len] = (uint32_t)task_info;
	
	if(prog_ptr != 0){
		fl_fclose(prog_ptr);
	}
	return(task_info->id);
	/*
	 * void* prog_ptr = fl_fopen*(name, "r");
	 * vector* pages_used = 0;
	 * struct task_data* task_info = 0;
	 * struct regs* machine_state = 0;
	 * uint8_t* kernel_stack = 0;
	 */
	cleanup:
	if(prog_ptr != 0){
		fl_fclose(prog_ptr);
	}
	if(pages_used != 0){
		for(uint16_t i = 0; i < pages_used->length; i+=2){
			dealloc_phys_page(pages_used->data[i+1]);
		}
		destroy_vector(pages_used);
	}
	if(task_info != 0){
		kfree(task_info);
	}
	if(machine_state != 0){
		kfree(machine_state);
	}
	if(kernel_stack != 0){
		kfree(kernel_stack);
	}
	return(0);
}

int load_program_and_execute(char* name){
	uint16_t tmp = load_program(name);
	print_string("TASK LOADED\n");
	hang();
	switch_to_task(tmp);
	return(1); //FIXTHIS
}