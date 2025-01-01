#include "../kernel.h"

void dump_mem(uint32_t start, uint32_t length){
	clear_screen();
	for(uint32_t i = 0; i< length; i++){
		hexbyte(*(uint8_t*)(i+start));
	}
	hang();
}