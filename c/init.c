#include "../kernel.h"

struct multiboot2_header{
	uint32_t total_size;
	uint32_t reserved;
}__attribute__((packed));

struct multiboot2_tag_top{
	uint32_t type;
	uint32_t size;
}__attribute__((packed));

struct multiboot2_framebuffer_info{
	uint64_t framebuffer_addr;
	uint32_t framebuffer_pitch;
	uint32_t framebuffer_width;
	uint32_t framebuffer_height;
	uint8_t framebuffer_bpp;
	uint8_t framebuffer_type;
	uint8_t resered;
	uint8_t colour_info[0];
}__attribute__((packed));

struct basic_mem_info{
	uint32_t mem_lower;
	uint32_t mem_upper;
}__attribute__((packed));

void parse_multiboot2(){
	uint32_t *asd = 0xc0000000;
	uint8_t* data_arr = multiboot2_space;
	uint16_t multiboot_tag_pointer = 0;
	struct multiboot2_header* multiboot2_data_header = (struct multiboot2_header*)&data_arr[multiboot_tag_pointer];
	multiboot_tag_pointer += 8;
	while(1){
		if(multiboot_tag_pointer > 0x1000 || multiboot_tag_pointer > multiboot2_data_header->total_size){
			return;
		}
		struct multiboot2_tag_top* current_tag = (struct multiboot2_tag_top*)&data_arr[multiboot_tag_pointer];
		void* data_addr = (void*)&data_arr[multiboot_tag_pointer+sizeof(struct multiboot2_tag_top)];
		switch(current_tag->type){
			case 0:
				return;
			default:
				break;
			case 8:		//video info
				struct multiboot2_framebuffer_info* framebuffer_info= data_addr;
				screen_address = (uint32_t*)(uint32_t)framebuffer_info->framebuffer_addr;
				x_res = framebuffer_info->framebuffer_width;
				y_res = framebuffer_info->framebuffer_height;
				break;
			case 4:		//basic memory information
				struct basic_mem_info* mem_info = data_addr;
				max_mem = mem_info->mem_upper;
				break;
		}
		multiboot_tag_pointer += current_tag->size;
		if(current_tag->size < 8){
			//This is bad
			hang();
		}
		multiboot_tag_pointer = (multiboot_tag_pointer+7)&(~7);
	}
}

void kernel_init(){
	
	gdt_init();
	
	parse_multiboot2();
	
	mem_init();
	
	graphics_init();
	print_string("GDT, Paging and graphics initialised\n");
	idt_init();
	print_string("IDT initialised\n");
	irq_init();
	print_string("IRQs INITIALISED\n");
	//timer init
	//keyboard init
	// disc_init();
	mono_disc_init();
	print_string("Disc controller initialised\n");
	init_mem_late();
	print_string("Finished memory initialisation with "); hexdword(HEAP_SIZE); print_string(" bytes of heap size\n");
	time_init();
	print_string("Timer Initialised\n");
	keyboard_init();
	print_string("Keyboard initialised\n");
	print_string("Special thanks to:\nDr Edwall - For getting me through my English GCSEs\nDr Kerney - ");
	print_string("For getting me through my History GCSE\nMrs Robinson - For getting me through my Biology ");
	print_string("GCSE\nMr Coakes - For getting me though my D&T GCSE\n");
	print_string("Herr Drozdov - for getting me through my German GCSE\n");
	print_string("Please don't be offended if you taught me and are not on this list. It means that I ");
	print_string("Liked your subject before being taught it, and therefore didn't need as much help from you. ");
	print_string("My results therefore do not show your (existant) teaching prowess");
	print_string("\nBoot completed. Press any key to continue.");
	clear_keyboard_buffer();
	while(!is_key_waiting());
	clear_keyboard_buffer();
	clear_screen();
	print_string("Welcome to ");
	print_string(version);
	print_string("\n");main();
	hang();
}
