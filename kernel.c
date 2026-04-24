#include "kernel.h"
#include "include.h"


void main(){
	//hexdword(free_pages_boot_count);newline();
	//hexdword(max_mem);
	//hang();
	// load_program_and_execute("/test_program_c.elf");
	// hang();
	shell();
	//load_program_and_execute("/test_program");
	hang();
	shell();
}