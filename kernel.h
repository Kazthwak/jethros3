#ifndef HEADER_GUARD
#define HEADER_GUARD
#define version "JETHROS3 Version 0.5"
//--------includes
#include <stdint.h>
#include <stdbool.h>
#include "c/font.h"
#include "c/scancodes.h"

//--------janks
#define dump_obj(obj_name) dump_mem((uint32_t)&obj_name, sizeof(obj_name));

//--------constants
//gdt
#define gdt_len 5
//for fixing janky pointers
#define PAGING_OFFSET 0xc0000000
//memory/paging
#define page_size (0x400*0x4) //4096b (4kb)
#define num_pages 0x100000 //number of 4kb pages in 4gb
#define num_page_entries (num_pages/32)
#define ONE_MB (0x400*0x400)
#define FOUR_MB (0x4*ONE_MB)
//memory allocation
#define SLAB_SIZE (64*ONE_MB)
//text
#define CHAR_WIDTH 8
#define CHAR_HEIGHT 16
//idt
#define idt_len 256
//disc
#define master_drive 0xE0
#define slave_drive  0xF0
//timer
#define TIMER_FREQUENCY 41
//keyboard
#define KEY_BUFFER_LENGTH 1024

//--------externs
extern uint32_t eax_boot;
extern uint32_t ebx_boot;
extern void* _kernel_start;
extern void* _kernel_end;
extern uint32_t page_directory_asm;
extern void* page_table_kernel_1;
extern void* page_table_kernel_2;
extern void* page_table_kernel_3;

extern void hang(void);
extern void hang_int(void);
void gdtr_load(void);
void idtr_load(void);
void gdt_load(void);
extern void page_reload(void);
extern void inton(void);
extern void testing(void);

extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr4(void);
extern void isr5(void);
extern void isr6(void);
extern void isr7(void);
extern void isr8(void);
extern void isr9(void);
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr15(void);
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);
extern void isr19(void);
extern void isr20(void);
extern void isr21(void);
extern void isr22(void);
extern void isr23(void);
extern void isr24(void);
extern void isr25(void);
extern void isr26(void);
extern void isr27(void);
extern void isr28(void);
extern void isr29(void);
extern void isr30(void);
extern void isr31(void);

extern void irq0(void);
extern void irq1(void);
extern void irq2(void);
extern void irq3(void);
extern void irq4(void);
extern void irq5(void);
extern void irq6(void);
extern void irq7(void);
extern void irq8(void);
extern void irq9(void);
extern void irq10(void);
extern void irq11(void);
extern void irq12(void);
extern void irq13(void);
extern void irq14(void);
extern void irq15(void);


//--------structs
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

struct idt_entry{
uint16_t addr_low;
uint16_t gdt_selector;
uint8_t reserved;
uint8_t flags;
uint16_t addr_high;
}__attribute__((packed));

struct idt_ptr{
uint16_t len;
uint32_t base;
}__attribute__((packed));

struct regs{
    volatile uint32_t gs, fs, es, ds;      /* pushed the segs last */
    volatile uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    volatile uint32_t int_no, err_code;    /* our 'push byte #' and ecodes do this */
    volatile uint32_t eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
}__attribute__((packed));

struct disc_sector{
	uint8_t data[512];
}__attribute__((packed));

struct keypress_data{
bool pressed;
uint8_t code;
}__attribute__((packed));

struct free_mem_link{
	uint32_t next_free_pointer;
	uint32_t length;
}__attribute__((packed));


//--------global variables
//gdt
struct gdt_entry gdt[gdt_len];
struct gdt_ptr gdtr;
struct tss_t tss;
//multiboot structs
struct MultiBootInfoStruct stateinfo;
struct vbe_mode_info vbe_info;
struct vbe_control_info vbe_control_info;
//graphics
uint16_t x_res;
uint16_t y_res;
uint16_t x_char_res;
uint16_t y_char_res;
uint16_t cursor_x;
uint16_t cursor_y;
uint32_t* screen_address;
volatile uint8_t framebuffer[768*0x400*4] __attribute__((aligned(4096)));
//memory
void* end_of_used_ram;
void* beg_of_used_ram;
uint32_t first_mem_hole;
uint32_t phys_page_state[num_page_entries];
uint32_t* page_directory = &page_directory_asm;
void* page_tables[1024];
//memory allocation
uint32_t slab_start;
uint32_t slab_end;
uint32_t first_free_slab;
//idt
struct idt_ptr idtr;
struct idt_entry idt_table[idt_len];
void* irq_handle_functions[16];
//timer
volatile uint64_t time = 0;
//keyboard
volatile struct keypress_data key_buffer[KEY_BUFFER_LENGTH];
volatile uint16_t key_buffer_pointer_bottom;
volatile uint16_t key_buffer_pointer_top;
uint32_t next_free_kernel_mem;


//--------prototypes
void memset(uint32_t base, uint8_t val, uint32_t length);
void memcpy(void* start, uint32_t length, void* dest);
void kernel_init(void);
void main(void);
void gdt_init(void);
void gdt_set_entry(uint8_t entry, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity);
void graphics_init(void);
void mem_init(void);
uint8_t init_page_valid(uint32_t mem_addr);
void init_phys_pages(void);
void* alloc_phys_page(uint32_t start_addr);
uint32_t get_phys_address(uint32_t address);
void map_page(uint32_t phys_addr, uint32_t virt_addr);
void clear_screen(void);
void putpixel(uint16_t x, uint16_t y, uint32_t colour);
void draw_rect(uint16_t x, uint16_t y, uint16_t x_size, uint16_t y_size, uint32_t colour);
void putcharxy(uint16_t x, uint16_t y, char character);
void putcharxyc(uint16_t x, uint16_t y, char character);
void newline(void);
void putchar(char character);
void print_string(char* string);
void idt_init(void);
void set_idt_entry(uint8_t entry, uint32_t addr, uint16_t gdt_selector, uint8_t flags);
void fault_handler(struct regs* r);
void hexdig(uint8_t num);
void hexbyte(uint8_t num);
void hexword(uint16_t num);
void hexdword(uint32_t num);
void hexqword(uint64_t num);
void binbyte(uint8_t num);
void disc_init(void);
bool disk_poll(void);
bool disk_read(volatile struct disc_sector* sector_address, uint32_t LBA);
void wordout(uint32_t port, uint16_t data);
uint16_t wordin(uint32_t port);
void byteout(uint32_t port, uint8_t data);
uint8_t bytein(uint16_t port);
void Qshutdown(void);
void irq_init(void);
void irq_handler(struct regs *r);
void pic_remap(void);
void IRQ_set_mask(unsigned char IRQline);
void IRQ_clear_mask(unsigned char IRQline);
void set_irq_handler(uint8_t num, void* function);
void clear_irq_handler(uint8_t num);
void time_init(void);
void set_timer_phase(int hz);
void timer_handle(struct regs* r);
uint64_t get_time_seconds(void);
void block_wait_secs(uint64_t ticks);
void block_wait_ticks(uint64_t ticks);
void keyboard_init(void);
void keyboard_handle(struct regs* r);
void block_wait_keyboard_read(void);
void block_wait_keyboard_write(void);
bool is_key_waiting(void);
struct keypress_data get_keypress(void);
void clear_keyboard_buffer(void);
uint32_t kmalloc_permanant(uint32_t length);
bool alloc_and_map_page(uint32_t virt_addr);
uint32_t kmalloc_permanant_page(void);
void init_mem_late(void);
void dump_mem(uint32_t start, uint32_t length);

#endif