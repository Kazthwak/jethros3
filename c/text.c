#include "../kernel.h"

void putchar(char character){
	if(character == '\n'){
		newline();
		return;
	}
	putcharxyc(cursor_x, cursor_y, character);
	cursor_x++;
	if(cursor_x >= x_char_res){
		newline();
	}
}

void newline(){
	cursor_y++;
	cursor_x = 0;
}

void print_string(char* string){
	while(*string != 0){
		putchar(*string);
		string++;
	}
}