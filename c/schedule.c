#include "../kernel.h"
//(just FYI if you are not me, schedule is pronounced with a soft sch (like german) in RP)

void schedule_tick(struct regs* r){
	// if(next_id != 0){hexbyte(tmp_bool);}
	if(tmp_bool && tasks != 0 && tasks->length >= next_id && next_id != 0){
		during_int_switch_to_task(r, next_id);
		clear_screen();
		tmp_bool = false;
	}
}