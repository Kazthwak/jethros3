#include "../kernel.h"

void putchar(char character){
	if(character == '\n'){
		newline();
		return;
	}
	putcharxyc(cursor_x, cursor_y, character);
	cursor_x++;
	if(cursor_x >= x_char_res-1){
		newline();
	}
	if(cursor_y >= y_char_res-1){
		//scroll
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

void hexdig(uint8_t num){
	uint8_t tmp = 0x30; //('0')
	if(num >= 0xa){
		tmp = 'A'-0xa;
	}
	putchar(tmp+num);
}

void hexbyte(uint8_t num){
	hexdig(num>>4);
	hexdig(num&0xf);
}

void hexword(uint16_t num){
	hexbyte(num>>8);
	hexbyte(num&0xff);
}

void hexdword(uint32_t num){
	hexword(num>>16);
	hexword(num&0xffff);
}

void hexqword(uint64_t num){
	hexdword(num>>32);
	hexdword(num&0xffffffff);
}

void binbyte(uint8_t num){
	//very janky loop. when it underflows, condition will be false
	for(uint8_t i = 7; i <= 7; i--){
		putchar(0x30+((num>>i)&1));
	}
}

void binword(uint16_t num){
	binbyte(num>>8);
	binbyte(num&0xff);
}

void fixed_decimal(uint16_t num){
	for(uint8_t i = 5; i > 0; i--){
		uint32_t tmp = pow(10, i);
		hexdig((num%tmp)*10/tmp);
	}
}

void cust_fixed_decimal(uint16_t num, uint8_t digs){
	for(uint8_t i = digs; i > 0; i--){
		uint32_t tmp = pow(10, i);
		hexdig((num%tmp)*10/tmp);
	}
}

void print_decimal(uint32_t num){
	uint8_t digs = 1;
	uint32_t tmp = num;
	while(1){
		tmp /= 10;
		if(tmp == 0){break;}
		digs += 1;
	}
	for(uint8_t i = digs; i > 0; i--){
		uint32_t tmp = pow(10, i);
		hexdig((num%tmp)*10/tmp);
	}
}