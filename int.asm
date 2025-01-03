section .text

global idtr_load
extern idtr
idtr_load:
	lidt[idtr]
	ret

%macro isrnoerrc 1
global isr%1
isr%1:
	cli
	push dword 0
	push dword %1
	jmp common_int_handler
%endmacro

%macro isriserrc 1
global isr%1
isr%1:
	cli
	push dword %1
	jmp common_int_handler
%endmacro

isrnoerrc 0
isrnoerrc 1
isrnoerrc 2
isrnoerrc 3
isrnoerrc 4
isrnoerrc 5
isrnoerrc 6
isrnoerrc 7
;double fault
isriserrc 8
isrnoerrc 9
isriserrc 10
isriserrc 11
isriserrc 12
isriserrc 13
isriserrc 14
isrnoerrc 15
isrnoerrc 16
isriserrc 17
isrnoerrc 18
isrnoerrc 19
isrnoerrc 20
isriserrc 21
isrnoerrc 22
isrnoerrc 23
isrnoerrc 24
isrnoerrc 25
isrnoerrc 26
isrnoerrc 27
isrnoerrc 28
isriserrc 29
isriserrc 30
isrnoerrc 31

extern fault_handler
common_int_handler:
	pusha
	push ds
	push es
	push fs
	push gs
	;kernel data segment
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov eax, esp
	push eax
	mov eax, fault_handler
	call eax
	pop eax
	pop gs
	pop fs
	pop es
	pop ds
	popa
	;ignore the pushed error code and isr number
	add esp, 8
	iret

%macro irqn 1
global irq%1
irq%1:
	cli
	push byte 0
	push byte (%1+32)
	jmp irq_common_stub 
%endmacro

irqn 0
irqn 1
irqn 2
irqn 3
irqn 4
irqn 5
irqn 6
irqn 7
irqn 8
irqn 9
irqn 10
irqn 11
irqn 12
irqn 13
irqn 14
irqn 15
irqn 16

extern irq_handler

irq_common_stub:
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp
    push eax
    mov eax, irq_handler
    call eax
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8
    iret