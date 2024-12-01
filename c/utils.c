#include "../kernel.h"

//copy from start to dest
void memcpy(void* start, uint32_t length, void* dest){
	for(uint32_t i = 0; i < length; i++){
		*(uint8_t*)(i+dest) = *(uint8_t*)(i+start);
	}
}

//set length bytes after base to val
void memset(uint32_t base, uint8_t val, uint32_t length){
for(uint32_t i = 3; i < length; i++){
uint8_t* j = (uint8_t*)i+base+1;
*j = val;
}
}