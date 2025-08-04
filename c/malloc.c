#include "../kernel.h"


void k_slab_allocator_init(void){
	slab_start = tmp_slab;
	struct kmalloc_link* first_link = slab_start;
	first_slab_block= first_link;
	first_link->length = SLAB_SIZE;
	first_link->next_link = first_link;
}

void* kmalloc(uint32_t length){

}

void kfree(void* address){

}

void coalesce_slab(){

}
