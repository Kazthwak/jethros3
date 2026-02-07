#include "../kernel.h"

void set_timer_phase(int hz){
    uint32_t divisor = 1193182 / hz;
    byteout(0x43, 0x36);
    byteout(0x40, divisor & 0xFF);
    byteout(0x40, divisor >> 8);
}

#define good_flicker
#define flicker

uint32_t stat = 0;
bool fl = false;;

void set_flick(bool state){
	fl = state;
	if(state == 0){
		stat = 0;
		draw_rect(cursor_x*CHAR_WIDTH, cursor_y*CHAR_HEIGHT, CHAR_WIDTH, CHAR_HEIGHT, stat);
	}
}



void timer_handle(__attribute__((unused)) struct regs* r){
	time++;
	#ifdef flicker
	if(fl){
		if(time%TIMER_FREQUENCY == 0){
			stat = stat^0xffffffff;
			#ifndef good_flicker
			draw_rect(cursor_x*CHAR_WIDTH, cursor_y*CHAR_HEIGHT, CHAR_WIDTH, CHAR_HEIGHT, stat);
			#endif
		}
		#ifdef good_flicker
		draw_rect(cursor_x*CHAR_WIDTH, cursor_y*CHAR_HEIGHT, CHAR_WIDTH, CHAR_HEIGHT, stat);
		#endif
		#endif
	}
}

void block_wait_secs(uint64_t ticks){
	block_wait_ticks(ticks*TIMER_FREQUENCY);
}

void block_wait_ticks(uint64_t ticks){
	uint64_t deadline = time+ticks;
	while(time<deadline){}
	return;
}

uint64_t get_time_seconds(){
	return(time/TIMER_FREQUENCY);
}

void time_init(){
	set_timer_phase(TIMER_FREQUENCY);
	set_irq_handler(0, (void*)timer_handle);
	IRQ_clear_mask(0);
}