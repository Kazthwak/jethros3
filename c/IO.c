#include "../kernel.h"

__attribute__((noreturn))
void Qshutdown(){
wordout(0x604, 0x2000);
__builtin_unreachable();
}

uint8_t bytein(uint16_t port){
	uint8_t ret;
	asm volatile ("inb %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}

void byteout(uint32_t port, uint8_t data){
asm volatile("out %% al, %% dx" : : "a" (data), "d" ((unsigned short)port));
}

uint16_t wordin(uint32_t port){
unsigned short result;
asm volatile("in %% dx , %% ax" : "=a" (result) : "d" ((unsigned short)port));
return result;
}

void wordout(uint32_t port, uint16_t data){
asm volatile("out %% ax , %% dx" : : "a" (data), "d" ((unsigned short)port));
}