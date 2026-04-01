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

global flush_tss
flush_tss:
	push ax
	mov ax, (5 * 8) ;the fifth 8 byte GDT entry (including the zeroeth null descriptor)
	ltr ax
	pop ax
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

global intoff
intoff:
	cli
	ret

global run_prog
run_prog:
	push ebp
	mov ebp, esp
	;sub esp, 0 ;make space on the stack for variables
	mov eax, [ebp+8]
	;do stuff
	pusha
	call eax
	popa
	
	leave ;clear up stack fram
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

global asm_iret
asm_iret:
	push ebp
	mov ebp, esp
; 	sub esp, 0 ;stack frame space
	;[ebp+8]  is address
	;[ebp+12] is stack
	
	;setup segments
	mov ax, (4 * 8) | 3 ; ring 3 data with bottom 2 bits set for ring 3
	mov ds, ax
	mov es, ax 
	mov fs, ax 
	mov gs, ax ; SS is handled by iret

	; set up the stack frame iret expects
	mov eax, esp
	push (4 * 8) | 3 ; data selector
	push eax ; current esp
	pushf ; eflags
	push (3 * 8) | 3 ; code selector (ring 3 code with bottom 2 bits set for ring 3)
	push [ebp+8] ; instruction address to return to
	iret
	
	leave
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