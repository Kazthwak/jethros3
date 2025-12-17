#include "kernel.h"
#include "include.h"

uint8_t spacea[1024];


void main(){
	print_string("ASD");
	void* test = fl_fopen("/test_program", "r");
	hexword(test); newline();
	hexdword(fl_fread(spacea, 2, 512, test)); newline();
	dump_obj(spacea);
	fl_fclose(test);
	hang();
	shell();
}