#include "../kernel.h"

struct task_data{
	vector* pages_used;
	struct regs* saved_regs;
	//uint32_t instruction_pointer;
	//uint32_t stack_pointer;
	uint32_t kernel_stack_pointer;
	uint16_t id;
};

vector* tasks;
uint16_t current_task = 0;