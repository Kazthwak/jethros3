#include "../kernel.h"

void kernel_init(){
gdt_init();

uint32_t tmp = ebx_boot+PAGING_OFFSET;
struct MultiBootInfoStruct* bootinfo = (struct MultiBootInfoStruct*)tmp;
stateinfo = *bootinfo;

//copy the vbe mode infostruct into a different one
tmp = stateinfo.vbe_mode_info+PAGING_OFFSET;
struct vbe_mode_info* minfo = (struct vbe_mode_info*)tmp;
vbe_info = *minfo;

//copy the vbe control infostruct into a different one
tmp = stateinfo.vbe_control_info+PAGING_OFFSET;
struct vbe_control_info* cinfo = (struct vbe_control_info*)tmp;
vbe_control_info = *cinfo;

mem_init();

graphics_init();
print_string("GDT, Paging and graphics initialised\n");
idt_init();
print_string("IDT initialised\n");
irq_init();
print_string("IRQs INITIALISED\n");
//timer init
//keyboard init
disc_init();
print_string("Disc controller initialised\n");
init_mem_late();
print_string("Finished memory initialisation\n");
time_init();
print_string("Timer Initialised\n");
keyboard_init();
print_string("Keyboard initialised\n");
print_string("\nBoot completed. Press any key to continue.");
clear_keyboard_buffer();
while(!is_key_waiting());
clear_keyboard_buffer();
clear_screen();
print_string("Welcome to ");
print_string(version);
print_string("\n");
main();
hang();
}