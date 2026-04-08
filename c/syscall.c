#include "../kernel.h"

uint32_t call_int0x30(uint32_t eax, uint32_t ebx, uint32_t ecx, uint32_t edx){
	asm volatile("int $0x30" : "=a" (eax) : "a" (eax), "b" (ebx), "c" (ecx), "d" (edx) : "memory");
	return(eax);
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
 *	Mode = 5: Task
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
 *	6: lengthed string (ebx is string ecx is length)
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
 *  TODO ADD SECURITY so that reads cannot overwrite kernel code...
 *	DISC ebx is always file pointer
 *	0: fopen - name is ebx, ecx is flags, edx is mode
 *	1: fclose
 *	2: fread - ecx is buffer, edx is length
 *	3: fwrite - ecx is buffer, edx is length
 *	4: fseek - ecx is offset, edx is whence
 *	5: fstat - ecx is stat
 *	
 *	TIMER
 *	
 *	TASK
 *	0: exit the current task
 *	1: execve(char *name, char **argv, char **env)
 *	2: int fork(void)
 *	3: getpid(void) returns pid
 *	4: isatty(int file) returns if the output is a terminal
 *	5: int kill(int pid, int sig)
 *	6: 
 *	7: 
 *	8: 
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
 *	EAX = 4: failiure in executing command
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
			case(6)://print length
				print_string_length((char*)r->ebx, r->ecx);
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
			switch(((r->eax)>>8)&0xff){
				default:
					r->eax = 2;
					return;
				case(0):
					//fopen
					//TODO fix this to properly use the flags (ecx) and mode (edx)
					char* mode = "r";
					r->ebx = (uint32_t)fl_fopen((char*)r->ebx, mode);
					r->eax = 0;
					return;
				case(1):
					//fclose
					fl_fclose((void*)r->ebx);
					r->eax = 0;
					return;
				case(2):
					//fread
					fl_fread((void*)r->ecx, 1, r->edx, (void*)r->ebx);
					r->eax = 0;
					return;
				case(3):
					//write
					fl_fwrite((void*)r->ecx, 1, r->edx, (void*)r->ebx);
					r->eax = 0;
					return;
				case(4):
					fl_fseek((void*)r->ebx, r->ecx, r->edx);
					r->ebx = fl_ftell((void*)r->ebx);
					r->eax = 0;
					return;
				case(5):
					//TODO FIXTHIS
					//fl_fstat(r->ebx, r->ecx);
					r->ebx = 0;
					r->eax = 0;
					return;
			}
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
		
		case(5):
			//task
			switch(((r->eax)>>8)&0xff){
				case(0):
					//exit the current program
					//TODO MAKE THIS EXIT
					print_string("_exit() called\n");
					hang();
					r->eax = 1;
					return;
				case(1):
					//execve(char *name, char **argv, char **env)
					hang();
					r->eax = 0;
					return;
				case(2):
					//int fork(void)
					r->ebx = current_task;
					r->eax = 0;
					fork(r);
					return;
				case(3):
					//getpid(void) returns pid
					r->ebx = current_task;
					r->eax = 1;
					return;
				case(4):
					//isatty(int file) returns if the output is a terminal
					r->ebx = (r->ebx <= 2);
					hexbyte(r->ebx);
					r->eax = 0;
					return;
				case(5):
					//int kill(int pid, int sig)
					if(r->ebx != current_task){
						destroy_process(r->ebx);
						r->eax = 0;
						return;
					}
					//self killed - care must be taken
					r->eax = 0;
					return;
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