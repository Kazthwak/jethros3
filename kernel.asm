section .text
global kernel_entry
extern kernel_init
kernel_entry:
	;nothing yet
	jmp kernel_init

global hang
hang:
	cli
	hlt
	jmp hang

global hang_int
hang_int:
	hlt
	jmp hang_int

global gdtr_load
extern gdtr
gdtr_load:
	lgdt[gdtr]
	ret


global gdt_load
gdt_load:
	jmp 0x08:.reloaded
	.reloaded:
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	ret

global page_reload
page_reload:
	push ecx
	mov ecx, cr3
	mov cr3, ecx
	pop ecx
	ret

global inton
inton:
	sti
	ret

extern test_var
global testing
testing:
	pusha
	mov dword ebx, [test_var]
	mov eax, 0
	int 0x30
	mov dword [test_var], eax
	popa
	ret

%include "int.asm"

section .bss
global stack_bottom
global stack_top
stack_bottom:
	resb 65536
stack_top:
global eax_boot
eax_boot:
	resb 4
global ebx_boot
ebx_boot:
	resb 4