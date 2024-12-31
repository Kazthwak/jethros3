#include "../kernel.h"

void idt_init(){
	set_idt_entry(0, (uint32_t)isr0, 0x08, 0x8e);
	set_idt_entry(1, (uint32_t)isr1, 0x08, 0x8e);
	set_idt_entry(2, (uint32_t)isr2, 0x08, 0x8e);
	set_idt_entry(3, (uint32_t)isr3, 0x08, 0x8e);
	set_idt_entry(4, (uint32_t)isr4, 0x08, 0x8e);
	set_idt_entry(5, (uint32_t)isr5, 0x08, 0x8e);
	set_idt_entry(6, (uint32_t)isr6, 0x08, 0x8e);
	set_idt_entry(7, (uint32_t)isr7, 0x08, 0x8e);
	set_idt_entry(8, (uint32_t)isr8, 0x08, 0x8e);
	set_idt_entry(9, (uint32_t)isr9, 0x08, 0x8e);
	set_idt_entry(10, (uint32_t)isr10, 0x08, 0x8e);
	set_idt_entry(11, (uint32_t)isr11, 0x08, 0x8e);
	set_idt_entry(12, (uint32_t)isr12, 0x08, 0x8e);
	set_idt_entry(13, (uint32_t)isr13, 0x08, 0x8e);
	set_idt_entry(14, (uint32_t)isr14, 0x08, 0x8e);
	set_idt_entry(15, (uint32_t)isr15, 0x08, 0x8e);
	set_idt_entry(16, (uint32_t)isr16, 0x08, 0x8e);
	set_idt_entry(17, (uint32_t)isr17, 0x08, 0x8e);
	set_idt_entry(18, (uint32_t)isr18, 0x08, 0x8e);
	set_idt_entry(19, (uint32_t)isr19, 0x08, 0x8e);
	set_idt_entry(20, (uint32_t)isr20, 0x08, 0x8e);
	set_idt_entry(21, (uint32_t)isr21, 0x08, 0x8e);
	set_idt_entry(22, (uint32_t)isr22, 0x08, 0x8e);
	set_idt_entry(23, (uint32_t)isr23, 0x08, 0x8e);
	set_idt_entry(24, (uint32_t)isr24, 0x08, 0x8e);
	set_idt_entry(25, (uint32_t)isr25, 0x08, 0x8e);
	set_idt_entry(26, (uint32_t)isr26, 0x08, 0x8e);
	set_idt_entry(27, (uint32_t)isr27, 0x08, 0x8e);
	set_idt_entry(28, (uint32_t)isr28, 0x08, 0x8e);
	set_idt_entry(29, (uint32_t)isr29, 0x08, 0x8e);
	set_idt_entry(30, (uint32_t)isr30, 0x08, 0x8e);
	set_idt_entry(31, (uint32_t)isr31, 0x08, 0x8e);
	idtr.base = (uint32_t)&idt_table;
	idtr.len = sizeof(idt_table)-1;
	idtr_load();
}

void set_idt_entry(uint8_t entry, uint32_t addr, uint16_t gdt_selector, uint8_t flags){
	idt_table[entry].addr_low = addr&0xffff;
	idt_table[entry].addr_high = addr>>16;

	idt_table[entry].gdt_selector = gdt_selector;
	idt_table[entry].reserved = 0;
	idt_table[entry].flags = flags;
}