#include "../kernel.h"

void fault_handler(struct regs* r){
	hexdword(r->int_no);
	hang();
}