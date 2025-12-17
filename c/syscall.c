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
			print_string("TSETSTESTSETSETSTESTSETSETSETSETSETSETSETSETETETSETETSETTEST");
			//Text
			r->eax = 2;
			break;
		case(1):
			//Non-text graphics
			r->eax = 2;
			break;
		case(2):
			//Keyboard input
			r->eax = 2;
			break;
		case(3):
			//Disc
			r->eax = 2;
			break;
		default:
			r->eax = 1;
			break;
	}
}