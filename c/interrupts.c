#include "../kernel.h"

void fault_handler(struct regs* r){
	//page fault
	if(r->int_no == 0x0e){
		//was a page not present fault
		if((r->err_code&1) == 0){
			//was not in user mode
			if(((r->err_code>>2)&1) == 0){
				newline();
				uint32_t address;
				asm volatile("mov %%cr2, %%eax" : "=a" (address):);
				uint32_t page_address = address&0xfffff000;
				//check if address is near a boandary. I am checking an excessive amount to avoid errors
				bool extra_page_needed = ((address+0x11)&0xfffff000) > page_address;
				//check if page already mapped
				uint32_t next_page_address = (((page_address>>12)+1)<<12);
				extra_page_needed &= (get_phys_address(next_page_address) == 0xffffffff);
				bool res = alloc_and_map_page(page_address);
				if(extra_page_needed){
					res &= alloc_and_map_page(next_page_address);
				}
				if(res){
					if(get_phys_address(address) == 0xffffffff){
						print_string("MAPPING FAILED");
						hang();
					}
					return;
				}
				print_string("ERROR, INSUFFICIENT MEMORY");
			}
		}
	}
	print_string("\n\nException number ");
	hexdword(r->int_no);
	print_string(" fired\n");
	hang();
}

void irq_init(){
	set_idt_entry(32, (uint32_t)irq0, 0x08, 0x8e);
	set_idt_entry(33, (uint32_t)irq1, 0x08, 0x8e);
	set_idt_entry(34, (uint32_t)irq2, 0x08, 0x8e);
	set_idt_entry(35, (uint32_t)irq3, 0x08, 0x8e);
	set_idt_entry(36, (uint32_t)irq4, 0x08, 0x8e);
	set_idt_entry(37, (uint32_t)irq5, 0x08, 0x8e);
	set_idt_entry(38, (uint32_t)irq6, 0x08, 0x8e);
	set_idt_entry(39, (uint32_t)irq7, 0x08, 0x8e);
	set_idt_entry(40, (uint32_t)irq8, 0x08, 0x8e);
	set_idt_entry(41, (uint32_t)irq9, 0x08, 0x8e);
	set_idt_entry(42, (uint32_t)irq10, 0x08, 0x8e);
	set_idt_entry(43, (uint32_t)irq11, 0x08, 0x8e);
	set_idt_entry(44, (uint32_t)irq12, 0x08, 0x8e);
	set_idt_entry(45, (uint32_t)irq13, 0x08, 0x8e);
	set_idt_entry(46, (uint32_t)irq14, 0x08, 0x8e);
	set_idt_entry(47, (uint32_t)irq15, 0x08, 0x8e);

	pic_remap();
	for(uint8_t i = 0; i <16; i++){IRQ_set_mask(i);}
	memset(&irq_handle_functions, 0, sizeof(irq_handle_functions));
	inton();
}

//irq num, interrupt number-32
void clear_irq_handler(uint8_t num){
	irq_handle_functions[num] = 0;
}

//irq num, interrupt number-32
void set_irq_handler(uint8_t num, void* function){
	irq_handle_functions[num] = function;
}

void irq_handler(struct regs *r){
	// print_string("Irq handler fired. Number ");
	// hexbyte(r->int_no);
	// hang();
	void (*handler)(struct regs *r);
	
	handler = irq_handle_functions[r->int_no - 32];
	if (handler){
		handler(r);
	}

	// but only if it is a pic interupt
	if(r->int_no < 48){
	
	if (r->int_no >= 40){
		byteout(0xA0, 0x20);
	}
	// interrupt controller too
	byteout(0x20, 0x20);
	}
}

void pic_remap(){
	byteout(0x20, 0x11);
	byteout(0xA0, 0x11);
	byteout(0x21, 0x20);
	byteout(0xA1, 0x28);
	byteout(0x21, 0x04);
	byteout(0xA1, 0x02);
	byteout(0x21, 0x01);
	byteout(0xA1, 0x01);
	byteout(0x21, 0x0);
	byteout(0xA1, 0x0);
}

//osdev wiki
void IRQ_set_mask(unsigned char IRQline){
    uint16_t port;
    uint8_t value;
 
    if(IRQline < 8) {
        port = 0x21;
    } else {
        port = 0xA1;
        IRQline -= 8;
    }
    value = bytein(port) | (1 << IRQline);
    byteout(port, value);        
}
 
 //osdev wiki
void IRQ_clear_mask(unsigned char IRQline){
    uint16_t port;
    uint8_t value;
 
    if(IRQline < 8) {
        port = 0x21;
    } else {
        port = 0xA1;
        IRQline -= 8;
    }
    value = bytein(port) & ~(1 << IRQline);
    byteout(port, value);        
}