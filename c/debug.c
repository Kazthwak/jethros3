#include "../kernel.h"

void dump_mem(uint32_t start, uint32_t length){
	clear_screen();
	for(uint32_t i = 0; i< length; i++){
		hexbyte(*(uint8_t*)(i+start)); space();
		byteout(0x3f8, *(uint8_t*)(i+start));
	}
	hang();
}

void test_keyboard(){
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

void error_can_continue(void){
#ifdef DEBUG_BUILD
	print_string("\nPress enter to continue...");
	clear_keyboard_buffer();
	while(1){
		while(!is_key_waiting()){}
		struct keypress_data press = get_keypress();
		if(press.code == KEY_ENTER){
			clear_keyboard_buffer();
			return;
		}
	}
#endif
	hang();
}

void space(){
	print_string(" ");
}