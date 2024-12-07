#include "../kernel.h"

void graphics_init(){
	screen_address = (uint32_t*)vbe_info.phys_addr;
	x_res = vbe_info.x_res;
	y_res = vbe_info.y_res;
	x_char_res = x_res/CHAR_WIDTH;
	y_char_res = y_res/CHAR_HEIGHT;
	uint32_t screen_size = (x_res*y_res)<<2;
	uint32_t screen_pages = screen_size/page_size;
	if(screen_pages > 768){
		hang();
	}
	for(uint16_t i = 0; i < (sizeof(framebuffer)/page_size); i++){
		map_page(((uint32_t)screen_address+(page_size*i)), ((uint32_t)&framebuffer+(page_size*i)));
	}
	clear_screen();
	cursor_x = 0;
	cursor_y = 0;
}

void clear_screen(){
	memset((uint32_t)&framebuffer, 0x0, sizeof(framebuffer));
}

void putpixel(uint16_t x, uint16_t y, uint32_t colour){
	if(x >= x_res || y >= y_res){return;}
	*(uint32_t*)((uint32_t)&framebuffer + ((x + (y*x_res))<<2)) = colour;
}

void draw_rect(uint16_t x, uint16_t y, uint16_t x_size, uint16_t y_size, uint32_t colour){
	if((x+x_size) >= x_res || (y+y_size) >= y_res){return;}
	uint32_t start_mem = (uint32_t)&framebuffer + ((x + (y*x_res))<<2);
	for(uint16_t v = 0; v < y_size; v++){
		for(uint16_t u = 0; u < x_size; u++){
			((uint32_t*)start_mem)[u] = colour;
		}
		start_mem += (x_res)<<2;
	}
}

void putcharxy(uint16_t x, uint16_t y, char character){
	if((x+CHAR_WIDTH) >= x_res || (y+CHAR_HEIGHT) >= y_res){return;}
	for(uint8_t i = 0; i < 8; i++){
		for(uint8_t j = 0; j < 8; j++){
			if((font_basic[(uint16_t)character][i]>>j)&1){
				putpixel(x+j, (y+i)<<1, 0xffffff);
				putpixel(x+j, ((y+i)<<1)+1, 0xffffff);
			}else{
				putpixel(x+j, (y+i)<<1, 0x0);
				putpixel(x+j, ((y+i)<<1)+1, 0x0);
			}
		}
	}
}

void putcharxyc(uint16_t x, uint16_t y, char character){
	putcharxy(x<<3, y<<3, character);
}
