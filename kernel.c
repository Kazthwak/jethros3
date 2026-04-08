#include "kernel.h"
#include "include.h"


void main(){
	load_program_and_execute("/test_program_c.elf");
	hang();
	shell();
	//load_program_and_execute("/test_program");
	hang();
	shell();
}