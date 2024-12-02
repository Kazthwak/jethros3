#ifndef HEADER_GUARD
#define HEADER_GUARD
//includes
#include <stdint.h>
#include <stdbool.h>

//constants
#define gdt_len 5
#define PAGING_OFFSET 0xc0000000

//structs
struct MultiBootInfoStruct{
	uint32_t flags;
	uint32_t mem_lower;
	uint32_t mem_upper;
	uint32_t boot_device;
	uint32_t cmdline;
	uint32_t mods_count;
	uint32_t mods_addr;
	uint32_t syms_1;
	uint32_t syms_2;
	uint32_t syms_3;
	uint32_t syms_4;
	uint32_t mmap_length;
	uint32_t mmap_addr;
	uint32_t drives_length;
	uint32_t drives_addr;
	uint32_t config_table;
	uint32_t boot_loader_name;
	uint32_t apm_table;
	//func 00
	uint32_t vbe_control_info;
	//func 01
	uint32_t vbe_mode_info;
	uint32_t vbe_mode;
	uint32_t vbe_interface_seg;
	uint32_t vbe_interface_off;
	uint32_t vbe_interface_len;
	uint32_t framebuffer_addr;
	uint32_t framebuffer_pitch;
	uint32_t framebuffer_width;
	uint32_t framebuffer_height;
	uint32_t framebuffer_bpp;
	uint32_t framebuffer_type;
	uint32_t colour_info;
}__attribute__((packed));

struct vbe_control_info{
	//VESA. NOT NULL TERMINATED
	char vesa_string[4];
	uint16_t vbe_ver;
}__attribute__((packed));

struct vbe_mode_info{
	uint16_t mode_attributes;
	uint8_t window_a_attributes;
	uint8_t window_b_attributes;
	uint16_t window_granularity;
	uint16_t window_size;
	uint16_t window_a_segment;
	uint16_t window_b_segment;
	uint32_t window_function_p;
	uint16_t bytes_scanline;
	uint16_t x_res;
	uint16_t y_res;
	uint8_t char_x;
	uint8_t char_y;
	uint8_t num_planes;
	uint8_t bits_per_pixel;
	uint8_t num_banks;
	uint8_t mem_model;
	uint8_t bank_size;
	uint8_t num_image_pages;
	uint8_t res_pages;
	uint8_t red_mask_size;
	uint8_t red_field_pos;
	uint8_t green_mask_size;
	uint8_t green_field_pos;
	uint8_t blue_mask_pos;
	uint8_t blue_field_pos;
	uint8_t rsvd_mask_size;
	uint8_t rsvd_field_position;
	uint8_t direct_colour_mode_info;
	//
	uint32_t phys_addr;
	uint32_t off_screen_mem_offset;
	uint32_t off_screen_mem_size;
}__attribute__((packed));

struct gdt_entry{
//low half of limit
uint16_t limit;
//low half of base
uint16_t base_low;
//second highest byte of base
uint8_t base_middle;
//access flags
uint8_t access;
//granularity flag
uint8_t granularity;
//highest byte of base
uint8_t base_high;
}__attribute__((packed));

struct gdt_ptr{
uint16_t limit;
uint32_t base;
}__attribute__((packed));

struct tss_t{
    uint16_t   link;
    uint16_t   link_h;

    uint32_t   esp0;
    uint16_t   ss0;
    uint16_t   ss0_h;

    uint32_t   esp1;
    uint16_t   ss1;
    uint16_t   ss1_h;

    uint32_t   esp2;
    uint16_t   ss2;
    uint16_t   ss2_h;

    uint32_t   cr3;
    uint32_t   eip;
    uint32_t   eflags;

    uint32_t   eax;
    uint32_t   ecx;
    uint32_t   edx;
    uint32_t    ebx;

    uint32_t   esp;
    uint32_t   ebp;

    uint32_t   esi;
    uint32_t   edi;

    uint16_t   es;
    uint16_t   es_h;

    uint16_t   cs;
    uint16_t   cs_h;

    uint16_t   ss;
    uint16_t   ss_h;

    uint16_t   ds;
    uint16_t   ds_h;

    uint16_t   fs;
    uint16_t   fs_h;

    uint16_t   gs;
    uint16_t   gs_h;

    uint16_t   ldt;
    uint16_t   ldt_h;

    uint16_t   trap;
    uint16_t   iomap;
}__attribute__((packed));

//global variables
struct gdt_entry gdt[gdt_len];
struct gdt_ptr gdtr;
struct tss_t tss;
struct MultiBootInfoStruct stateinfo;
struct vbe_mode_info vbe_info;
struct vbe_control_info vbe_control_info;

//prototypes
void memset(uint32_t base, uint8_t val, uint32_t length);
void memcpy(void* start, uint32_t length, void* dest);
void kernel_init(void);
void main(void);
void gdt_init(void);
void gdt_set_entry(uint8_t entry, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity);
void graphics_init(void);

//externs
extern uint32_t eax_boot;
extern uint32_t ebx_boot;

extern void hang(void);
extern void hang_int(void);
void gdtr_load(void);
void idtr_load(void);
void gdt_load(void);

#endif