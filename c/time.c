#include "../kernel.h"

void set_timer_phase(int hz){
    uint32_t divisor = 1193182 / hz;
    byteout(0x43, 0x36);
    byteout(0x40, divisor & 0xFF);
    byteout(0x40, divisor >> 8);
}

void timer_handle(__attribute__((unused)) struct regs* r){
	time++;
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