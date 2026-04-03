#include "../kernel.h"

void task_init(){
	tasks = new_vector();
}

void iret_to_address(struct regs* r){
	asm_iret(r);
}

//honestly I don't think this function is good for anything, so maybe don't fill with params
void new_task_gen(/*TODO: Fill with params*/){
	
}

void update_current_task(struct regs* r){
	if(current_task == 0){
		print_string("NO CURRENT TASK");
		return;
	}
	if(tasks == 0){
		print_string("ERROR. SWITCHING FROM TASK WITH NULL TASKS ARRAY");
		return;
	}
	for(uint16_t i = 0; i < tasks->length; i++){
		if(((struct task_data*)tasks->data[i])->id == current_task){
		memcpy(((struct task_data*)tasks->data[i])->saved_regs, r, sizeof(struct regs));
		return;
		}
	}
	print_string("ERROR, OLD TASK NOT FOUND");
}

//This is basicall a copy of the other one
void during_int_switch_to_task(struct regs* r, uint16_t id){
	/*
	newline();
	hexdword(((struct task_data*)tasks->data[0])->pages_used);
	newline();
	hexdword(((struct task_data*)tasks->data[1])->pages_used);
	newline();
	newline();
	dump_vector(((struct task_data*)tasks->data[0])->pages_used);
	newline();newline();
	dump_vector(((struct task_data*)tasks->data[1])->pages_used);
	hang();//*/
	for(uint16_t i = 0; i < tasks->length; i++){
		if(((struct task_data*)tasks->data[i])->id == id){
			//correct task found
			//TODO add safety checks for invalid data
			struct task_data* target_task = (struct task_data*)tasks->data[i];
		
			
			//backup register state for old process
			update_current_task(r);
			current_task = id;
			
			//load pages - it is safe to clobber memory because the previous task has (hopefully) saved anything that needed to be saved
			vector* pages_to_map = target_task->pages_used;
			for(uint32_t j = 0; j < pages_to_map->length; j += 2){
				map_page(pages_to_map->data[j+1], pages_to_map->data[j]);
			}
			//flush the page cache to avoid bugs
			page_reload();
			//setup kernel stack
			update_tss_stack_ptr(0x10, target_task->kernel_stack_pointer);
			//Switch to it (but with the regs obj you have as a pointer this time)
			memcpy(r, target_task->saved_regs, sizeof(struct regs));
			if(id == 3 && 0){
				newline();
				hexdword(r->useresp); newline();
				hexdword(r->eip); newline();
				hexdword(get_phys_address(0x400000));newline();
				hexdword(*(uint32_t*)0x400000);
				hang();
			}
			return;
		}
	}
	print_string("ERROR WHILE SWITCHING TASK. TASK NOT FOUND");
	hang();
}

void switch_to_task(uint16_t id){
	for(uint16_t i = 0; i < tasks->length; i++){
		if(((struct task_data*)tasks->data[i])->id == id){
			//correct task found
			//TODO add safety checks for invalid data
			struct task_data* target_task = (struct task_data*)tasks->data[i];
			//load pages - it is safe to clobber memory because the previous task has (hopefully) saved anything that needed to be saved
			vector* pages_to_map = target_task->pages_used;
			for(uint32_t j = 0; j < pages_to_map->length; j += 2){
				map_page(pages_to_map->data[j+1], pages_to_map->data[j]);
			}
			//flush the page cache to avoid bugs
			page_reload();
			//setup kernel stack
			update_tss_stack_ptr(0x10, target_task->kernel_stack_pointer);
			//Switch to it
			current_task = id;
			iret_to_address(target_task->saved_regs);
		}
	}
	print_string("ERROR WHILE SWITCHING TASK> TASK NOT FOUND");
	hang();
}

//TODO make this better. keeping a list of free IDs is probably better, but for now this is easier
//also if the last used slot is freed while every other slot is full it will falsely return 0 (no free slots)
uint16_t find_free_task_id(){
	static uint16_t last_free = 0;
	if(tasks->length == 0){
		last_free = 1;
		return(1);
	}
	//go through all oxffff ids jankily
	for(uint16_t i = 0; i++ != 0xffff;){
		//this will wrap around, which is good so that all ids are checked
		uint16_t candidate = (uint16_t)(i+last_free);
		if(candidate == 0){
			continue;
		}
		//check if ID is used by a task
		for(uint16_t j = 0; j < tasks->length; j++){
			if(((struct task_data*)tasks->data[j])->id == candidate){
				break;
			}
			//the last task in the list did not have this ID, so the id is free
			if(j == tasks->length - 1){
				uint16_t found = candidate;
				last_free = found;
				return(found);
			}
		}
	}
	return(0);
}