#include "kernel.h"
#include "include.h"

uint8_t spacea[1024];


void main(){
	shell();
	load_program_and_execute("/test_program");
	hang();
	shell();
}