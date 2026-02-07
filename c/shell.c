#include "../kernel.h"

void shell(){
	clear_screen();
	print_string(version  " SHELL\nTYPE COMMAND");
	set_flick(true);
	while(1){
		char buffer[128] = {0};
		uint8_t buffer_next = 1;
		buffer[0] = '/';
		newline();
		print_string(">");
		uint16_t begx = cursor_x;
		uint16_t begy = cursor_y;
		while(1){
			while(!is_key_waiting()){};
			struct keypress_data keyd = get_keypress();
			if(keyd.pressed == false){continue;}
			if(keyd.code == KEY_ENTER){break;}
			if(keyd.code == KEY_BACKSPACE && cursor_y >= begy && (cursor_x > begx || cursor_y > begy)){
				if(cursor_x == 0){
					cursor_y--;
					cursor_x = x_char_res-1;
				}else{
					cursor_x--;
				}
				putcharxyc(cursor_x, cursor_y, 0x0);
				putcharxyc(cursor_x+1, cursor_y, 0x0);
				if(buffer_next >= 1){
					buffer_next--;
					buffer[buffer_next] = 0;
				}
			}
			if(is_ascii(keyd.code)){
				buffer[buffer_next] = get_ascii(keyd.code);
				buffer_next++;
				putchar(get_ascii(keyd.code));
			}
		}
		newline();
		uint8_t tmp = load_program_and_execute(buffer);
		hexbyte(tmp);
	}
}