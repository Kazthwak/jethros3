//unfortunately prog is short for programme, not progressive rock
#include "../kernel.h"

#define ARBITRARY_SIZE_LIMIT (8*0x400) //(8KB)
#define PROG_LOAD_LOC (0x1*0x400) //1KB

uint8_t* prog_loc = (uint8_t*)PROG_LOAD_LOC;

int load_program_and_execute(char* name){
	void* prog_ptr = fl_fopen(name, "r");
	if(prog_ptr == 0){return(0);}
	fl_fseek(prog_ptr, 0, SEEK_END);
	uint32_t size = fl_ftell(prog_ptr);
	fl_fseek(prog_ptr, 0, SEEK_SET);
	if(size > ARBITRARY_SIZE_LIMIT){return(0);}
	uint32_t tmp = fl_fread(prog_loc, 1, size, prog_ptr);
	if(tmp != size){return(0);}
	run_prog((void*)PROG_LOAD_LOC);
	return(1);
}