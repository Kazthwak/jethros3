#include "kernel.h"
#include "include.h"

struct disc_sector aaa;

void main(){
	clear_keyboard_buffer();
	while(true){
		while(!is_key_waiting()){}
		struct keypress_data tmp = get_keypress();
		binbyte((*(uint16_t*)&tmp)>>8);
		putchar(' ');
		hexbyte((*(uint16_t*)&tmp)&0xff);
		putchar(' ');
		putchar(' ');
	}
}
