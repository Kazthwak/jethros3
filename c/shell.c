#include "../kernel.h"

void shell(){
	
	clear_screen();
	print_string(version  " SHELL\nTYPE COMMAND");
	while(1){
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
			}
			if(is_ascii(keyd.code)){
				putchar(get_ascii(keyd.code));
			}
		}
	}
}