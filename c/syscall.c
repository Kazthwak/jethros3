#include "../kernel.h"

uint32_t call_int0x30(uint32_t eax, uint32_t ebx, uint32_t ecx, uint32_t edx){
	uint32_t tmp = 0;
	asm volatile("int $0x30" : "=a" (eax) : "a" (eax), "b" (ebx), "c" (ecx), "d" (edx));
	return(tmp);
}

/*	EAX is the mode, EBX, ECX and EDX are paramaters. EAX is primary return value
 *	high 2 bytes (eax-ax) is mode
 *	next byte (ah) is command
 *	next byte (al) is sub-command
 *	Mode = 0: Text
 *	Mode = 1: Non-text graphics
 *	Mode = 2: Keyboard input
 *	Mode = 3: disc
 *	Mode = 4: Timer
 *	Mode = 5: Multitasking //For the future
 *	
 *	TEXT
 *	0: print the string in ebx
 *	1: print the char in bl
 *	2: set the cursor to x=ebx, y=ecx
 *	3: clear the screen
 *	4: set cursor flicker state to ebx
 *	5: print number in registers (ebx, then (ecx<<32) | ebx if bit 8 of al set):
 *		0: hex byte
 *		1: bin byte
 *		2: hex word
 *		3: bin word
 *		4: hex dword
 *		5: hex qword
 *		6: 16_bit decimal
 *		7: expanding decimal
 * 
 *	GRAPHICS
 *	
 *	KEYBOARD
 *	0: is key waiting
 *	1: get keypress data
 *	2: is_ascii
 *	3: get_ascii
 *	4: clear buffer
 *	
 *	DISC
 *	
 *	TIMER
 *	
 *	
 *	
 *	
 *	
 *	
 *	
 *	
 *	
 *	
 *	
 *	
 *	Return Values:
 *	EAX = 0: OK
 *	EAX = 1: unsupported mode
 *	EAX = 2: unsupported command
 *	EAX = 3: unsupported sub-command
 *	EAX >=4: other failiure
 *	
 *	
 */

void int0x30handle(struct regs* r){
	switch((r->eax)>>16){
		case(0):
			//Text
			switch(((r->eax)>>8)&0xff){
				case(0)://string
					print_string((char*)r->ebx);
					break;
				case(1)://char
					putchar((char)r->ebx&0xff);
					break;
				case(2)://set x,y
					cursor_x = r->ebx;
					cursor_y = r->ecx;
					break;
				case(3)://clear screen
					clear_screen();
					break;
				case(4)://change cursor state
					set_flick(r->ebx);
					break;
				case(5)://number printing
					uint64_t num = 0;
					if(r->eax & 0x80){
						num = ((uint64_t)(r->ecx)<<32) | r->ebx;
					}else{
						num = r->ebx;
					}
					switch((r->eax)&0x7f){
						case(0)://hex byte
							hexbyte(num);
							break;
						case(1)://binbyte
							binbyte(num);
							break;
						case(2)://hexword
							hexword(num);
							break;
						case(3)://binword
							binword(num);
							break;
						case(4)://hexdword
							hexdword(num);
							break;
						case(5)://hexqword
							hexqword(num);
							break;
						case(6)://fixed decimal
							fixed_decimal(num);
							break;
						case(7)://decimal
							print_decimal(num);
							break;
					}
					break;
				default:
					r->eax = 2;
					return;
			}
			r->eax = 0;
			return;
		case(1):
			//Non-text graphics
			r->eax = 2;
			break;
		case(2):
			//Keyboard input
			switch(((r->eax)>>8)&0xff){
				default:
					r->eax = 2;
					return;
				case(0):
					r->ebx = is_key_waiting();
					break;
				case(1):
					struct keypress_data a= get_keypress();
					r->ebx = *(uint16_t*)&a;
					break;
				case(2):
					r->ebx = is_ascii(r->ebx);
					break;
				case(3):
					r->ebx = get_ascii(r->ebx);
					break;
				case(4):
					clear_keyboard_buffer();
					break;
			}
			r->eax = 0;
			return;
		case(3):
			//Disc
			r->eax = 2;
			break;
		case(4):
			//timer
			switch(((r->eax)>>8)&0xff){
				default:
					r->eax = 2;
					return;
			}
			r->eax = 0;
			break;
		default:
			r->eax = 1;
			break;
	}
}