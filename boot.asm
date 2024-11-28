MBALIGN  equ  1 << 0            ; align loaded modules on page boundaries
MEMINFO  equ  1 << 1            ; provide memory map
VIDEO    equ  1 << 2            ; change video mode
MBFLAGS  equ  MBALIGN | MEMINFO | VIDEO; this is the Multiboot 'flag' field
MAGIC    equ  0x1BADB002        ; 'magic number' lets bootloader find the header
CHECKSUM equ -(MAGIC + MBFLAGS)   ; checksum of above, to prove we are multiboot

BEG_OFFSET equ 0xC0000000		;The offset to be subtracted before paging is set up

; Declare a multiboot header that marks the program as a kernel. These are magic
; values that are documented in the multiboot standard. The bootloader will
; search for this signature in the first 8 KiB of the kernel file, aligned at a
; 32-bit boundary. The signature is in its own section so the header can be
; forced to be within the first 8 KiB of the kernel file.
section .multiboot.data
align 4
	dd MAGIC
	dd MBFLAGS
	dd CHECKSUM
	dd 0
	dd 0
	dd 0
	dd 0
	dd 0
	;graphics
	;mode
	dd 0
	;width
	dd 1024
	;height
	dd 768
	;depth
	dd 0

;stack
section .boostrap_stack
stack_bottom:
	times 16348 db 0; 16kB
stack_top:

;page tables
section .bss
align 4096
global page_directory
page_directory:
resb 4096; 4KB page directory
global page_table_kernel_1
page_table_kernel_1:
resb 4096 ; 4KB page table

section .multiboot.text
global _start
extern eax_boot
_start:
	mov [eax_boot-BEG_OFFSET], eax
	;start the jank
	;fill beggining of page directory
	;fill first page table
	;enable paging
	;jump to the kernel
	;Do you think it will triple fault because no GDT
