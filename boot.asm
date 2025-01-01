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

;page tables
section .bss
align 4096
global page_directory_asm
page_directory_asm:
resb 4096; 4KB page directory
global page_table_kernel_1
page_table_kernel_1:
resb 4096 ; 4KB page table
global page_table_kernel_2
page_table_kernel_2:
resb 4096 ; 4KB page table
global page_table_kernel_3
page_table_kernel_3:
resb 4096 ; 4KB page table
;3 page tables done here, 768, 769 and 770

section .multiboot.text
global _start
extern eax_boot
extern ebx_boot
extern kernel_entry
_start:
	mov [eax_boot-BEG_OFFSET], eax
	mov [ebx_boot-BEG_OFFSET], ebx
	;start the jank
	;fill beginning of page directory
		;empty it
		;mov eax, [page_directory - BEG_OFFSET]
		;dir_clear:
		;	cmp eax, (page_directory+4096 - BEG_OFFSET)
		;	jge end_dir_clear
		;	mov [0x0], long eax
		;	;mov [eax], long 0x0
		;	add eax, 0x4
		;jmp dir_clear
		;end_dir_clear:
		;jmp $
		;int 0
		;page directory empty BUT WHAT IF NOT EMPTY. THEN MIGHT IT WORK?
		;set the first element of it to be a pointer to the page table
		mov eax, (page_table_kernel_1 - BEG_OFFSET)
		or eax, 0b11
		mov [page_directory_asm - BEG_OFFSET], eax
		;whichever Genius came up with the idea to mape the page table in twice, deserves a nobel prize
		mov [page_directory_asm - BEG_OFFSET + (768 * 4)], eax
		;the second page table
		mov eax, (page_table_kernel_2 - BEG_OFFSET)
		or eax, 0b11
		mov [page_directory_asm - BEG_OFFSET + (769 * 4)], eax
		;the third page table
		mov eax, (page_table_kernel_3 - BEG_OFFSET)
		or eax, 0b11
		mov [page_directory_asm - BEG_OFFSET + (770 * 4)], eax
		;This has worked, right?
	;fill first page table
	mov eax, 0
	;eax is the counter
	mov ebx, (page_table_kernel_1 - BEG_OFFSET)
	;ebx is the pointer to the element of the page table. (EAX and EBX could be amalgamated, but I cannot be bothered)
		fill_first_table:
		cmp eax, 1023
		jg fill_first_table_end
		;update the correct area of memory
		mov ecx, eax
		;multiply by 0x1000
		shl ecx, 0xc
		or ecx, 3
		mov [ebx], ecx
		;ebx now points to the next entry
		add ebx, 4
		;loop
		inc eax
		jmp fill_first_table
	fill_first_table_end:
	mov eax, 1024
	;eax is the counter
	mov ebx, (page_table_kernel_2 - BEG_OFFSET)
	;ebx is the pointer to the element of the page table. (EAX and EBX could be amalgamated, but I cannot be bothered)
		fill_second_table:
		cmp eax, 2047
		jg fill_second_table_end
		;update the correct area of memory
		mov ecx, eax
		;multiply by 0x1000
		shl ecx, 0xc
		or ecx, 3
		mov [ebx], ecx
		;ebx now points to the next entry
		add ebx, 4
		;loop
		inc eax
		jmp fill_second_table
	fill_second_table_end:
	mov eax, 2048
	;eax is the counter
	mov ebx, (page_table_kernel_3 - BEG_OFFSET)
	;ebx is the pointer to the element of the page table. (EAX and EBX could be amalgamated, but I cannot be bothered)
		fill_third_table:
		cmp eax, 3071
		jg fill_third_table_end
		;update the correct area of memory
		mov ecx, eax
		;multiply by 0x1000
		shl ecx, 0xc
		or ecx, 3
		mov [ebx], ecx
		;ebx now points to the next entry
		add ebx, 4
		;loop
		inc eax
		jmp fill_third_table
	fill_third_table_end:
	;enable paging
	;set pointer to page directory
	mov ecx, (page_directory_asm-BEG_OFFSET)
	mov cr3, ecx
	;turn on paging
	mov ecx, cr0
	or ecx, 0x80010000
	mov cr0, ecx
	;jump to the kernel
	mov eax, finish_paging
	jmp eax

section .text

extern stack_bottom
extern stack_top
finish_paging:
	;remove the identity mapping thing
	mov [page_directory_asm], long 0x0
	;force a reload of the paging structures
	mov ecx, cr3
	mov cr3, ecx
	;setup the proper stack
	mov esp, stack_top
	jmp kernel_entry
