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

main();
hang();
}