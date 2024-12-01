section .text
global kernel_entry
extern main
kernel_entry:
	;nothing yet
	jmp main

global hang
hang:
	cli
	hlt
	jmp hang

global hang_int
hang_int:
	hlt
	jmp hang_int

section .bss
global stack_bottom
global stack_top
stack_bottom:
	resb 65536
stack_top:
global eax_boot
eax_boot:
	resb 4