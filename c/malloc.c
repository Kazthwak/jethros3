#include "../kernel.h"


void k_slab_allocator_init(void){
	slab_start = (uint32_t)tmp_slab;
	struct kmalloc_link* first_link = (struct kmalloc_link*)slab_start;
	first_slab_block = (uint32_t)first_link;
	first_link->length = SLAB_SIZE;
	first_link->next_link = first_link;
}

struct alloc_tmp_str{
	uint32_t length;
	uint8_t data[0];
}__attribute__((packed));

//Load the first link. If it is large enough
void* kmalloc(uint32_t length){
	//length must be >=4 bytes so that there is space to fit a link when it is freed
	if(length < 4){
		return(0x0);
	}
	uint32_t needed_length = length + sizeof(struct alloc_tmp_str);
	struct kmalloc_link* current_link = (struct kmalloc_link*)first_slab_block;
	//inintial variables have been setup. Next, I need to traverse the linked list
	while(1){
		//check if the current link will do by seeing if it is long enough to contain both the header for the link and the data itself
		if(current_link->length >= needed_length + sizeof(struct kmalloc_link)){
			break;
		}
		//if not, and the current link points back to the first link, return 0, a null pointer
		if((uint32_t)current_link->next_link == first_slab_block){
			return(0x0);
		}
		//large enough block has not been found, but there are more blocks. traverse the list
		current_link = current_link->next_link;
	}
	//current link points to a link of sufficient size. Continue with allocation
	//find the place for the data
	//calculate the new value for the link's length'
	uint32_t unused_space = current_link->length - needed_length;
	//create a struct for the section of memory that will be returned at the correct space
	struct alloc_tmp_str* allocated_memory = (struct alloc_tmp_str*)((uint32_t)current_link+unused_space);
	//update the struct's length so that it can be freed later
	allocated_memory->length = needed_length;
	//update the link
	current_link->length = unused_space;
	return(&(allocated_memory->data));
}


//coalescing could be merged with new link creation, to avoid unecassary overhead, but this is too minimal a performance imrpovement for the effort
void kfree(void* address){
	//work out the true address of the struct
	uint32_t true_address = (uint32_t)address - sizeof(struct alloc_tmp_str);
	struct alloc_tmp_str* alloced_struct = (struct alloc_tmp_str*)true_address;
	//get the first link
	struct kmalloc_link* current_link = (struct kmalloc_link*)first_slab_block;
	//traverse the structure, until the current link (is before the address to be freed (this should be implied by the search method), and) points to after the address (or back to the first link)
	while(1){
		//Link loops back to beginning or past the address being freed
		if((uint32_t)(current_link->next_link) == first_slab_block || (uint32_t)(current_link->next_link) > true_address){
			break;
		}
		//goto the next link
		current_link = current_link->next_link;
	}
	//current link is the one directly preceeding the address being freed
	//make the new link
	struct kmalloc_link* new_link = (struct kmalloc_link*)true_address;
	//populate it
	new_link->length = alloced_struct->length;
	new_link->next_link = current_link->next_link;
	//update previous link to point to this one
	current_link->next_link = new_link;
	//now coalesce.
	//only two things need to be checked. The creation of a new link can only border 2 other links
	//check if there is a link where this one ends
	if((uint32_t)(new_link->next_link) == (uint32_t)(new_link)+(uint32_t)(new_link->length)){
		//update properties to include the next link
		new_link->length = new_link->length + (new_link->next_link)->length;
		new_link->next_link = (new_link->next_link)->next_link;
	}
	//check if it is the last link
	if(new_link->length + (uint32_t)new_link > slab_start+SLAB_SIZE || (new_link->length + (uint32_t)new_link == slab_start+SLAB_SIZE && (uint32_t)new_link->next_link != first_slab_block)){
		print_string("ERROR, COALESCING OF SLAB BLOCK RESULTED IN BLOCK PAST SLAB BOUNDS OR TERMINATING BLOCK NOT POINTING TO INITIAL BLOCK");
		error_can_continue();
	}
	//if the previeous link ends flush with the new link
	if(current_link->length + (uint32_t)current_link == (uint32_t)new_link){
		//update it so the new link is unused. Smarter code would put this check higher up, but by putting it after the previous check, the code can handle the case where both sides of the new link face a link
		current_link->length = current_link->length + new_link->length;
		current_link->next_link = new_link->next_link;
	}
	//memory has been freed, and the links concatenated if possible. Now I just have to hope there are no memory leaks.
	//This implementation is kind of flawed. Although not a leak, the code can end up with lots of 8 length links (so they cannot be allocaed). This is very abusable as long as you know the slab size
}