#include "../kernel.h"

// #define BAD_KEYBOARD

void keyboard_handle(__attribute__((unused)) struct regs* r){
static struct keypress_data key_data = {true, 0};
uint8_t data = bytein(0x60);
//check if it was a 0xf0 to mean the key was released
if(data == 0xf0){
	key_data.pressed = false;
	return;
}
#ifdef BAD_KEYBOARD
key_data.code = data;
#else
key_data.code = scancode_lookup_table[data];
#endif
if((key_buffer_pointer_top+1)%KEY_BUFFER_LENGTH == key_buffer_pointer_bottom){
	return;
	//buffer "full"
}
//put in the buffer
key_buffer[key_buffer_pointer_top] = key_data;
key_buffer_pointer_top = (key_buffer_pointer_top+1)%KEY_BUFFER_LENGTH;
key_data.pressed = true;
}

bool is_key_waiting(){
	return(key_buffer_pointer_bottom != key_buffer_pointer_top);
}

struct keypress_data get_keypress(){
	if(key_buffer_pointer_bottom == key_buffer_pointer_top){return((struct keypress_data){false, 0xff});}
	struct keypress_data tmp = key_buffer[key_buffer_pointer_bottom];
	key_buffer_pointer_bottom = (key_buffer_pointer_bottom+1)%KEY_BUFFER_LENGTH;
	return(tmp);
}

bool is_ascii(uint8_t code){
	uint8_t tmp = get_ascii(code);
	return(tmp != 0 && (tmp>>7) == 0);
}

char get_ascii(uint8_t code){
	return(ascii_id_lookup_table[code]);
}

void block_wait_keyboard_read(){
	while(!(bytein(0x64)&1)){
	}
}

void block_wait_keyboard_write(){
	while((bytein(0x64)>>1)&1){}
}

void clear_keyboard_buffer(){
	key_buffer_pointer_bottom = 0;
	key_buffer_pointer_top = 0;
}

void keyboard_init(){
	//turn off translation
	//disable devices 1 and 2
	byteout(0x64, 0xad);
	block_wait_keyboard_write();
	byteout(0x64, 0xa7);
	//flush buffer
	bytein(0x60);
	//change config to disable translation
	//send get config command
	block_wait_keyboard_write();
	byteout(0x64, 0x20);
	//get the config in
	block_wait_keyboard_read();
	uint8_t config = bytein(0x60);
	config &= 0b10111111;
//	config = 0b00000101;
	//send set config command
	block_wait_keyboard_write();
	byteout(0x64, 0x60);
	//send config byte
	block_wait_keyboard_write();
	byteout(0x60, config);
	//read back the config byte
	block_wait_keyboard_write();
	byteout(0x64, 0x20);
	block_wait_keyboard_read();
	//enable device 1
	block_wait_keyboard_write();
	byteout(0x64, 0xae);
	//log the config byte
	block_wait_keyboard_write();
	byteout(0x64, 0x20);
	block_wait_keyboard_read();
	(bytein(0x60));
	//reset keyboard
	byteout(0x60, 0xff);
	block_wait_keyboard_read();
	if(bytein(0x60) != 0xfa){
		print_string("Reply was not an ACK");
		hang();
	}
	block_wait_keyboard_read();
	if(bytein(0x60) != 0xaa){
		print_string("Keyboard self test failed");
		hang();
	}
	while(1){
		//switch scancode
		block_wait_keyboard_write();
		byteout(0x60, 0xf0);
		block_wait_keyboard_write();
		//to scancode 3
		byteout(0x60, 0x03);
		block_wait_keyboard_read();
		if(bytein(0x60) == 0xfa){break;}
	}
	while(1){
		//get/set scancode
		block_wait_keyboard_write();
		byteout(0x60, 0xf0);
		//don't set, get
		block_wait_keyboard_write();
		byteout(0x60, 0x00);
		//read it back, check for an ACK
		block_wait_keyboard_read();
		uint8_t response = bytein(0x60);
		if(response == 0xfa){break;}
	}
	//check it is scancode 3
	block_wait_keyboard_read();
	uint8_t response = bytein(0x60);
	if(response != 0x3){
		print_string("SCANCODE 3 NOT SUPPORTED, HALTING");
		hang();
	}
	//clear hardware buffer
	bytein(0x60);
	//clear the software buffer
	key_buffer_pointer_bottom = 0;
	key_buffer_pointer_top = 0;
	//enable interrupts
	set_irq_handler(1, (void*)keyboard_handle);
	IRQ_clear_mask(1);
}
