typedef struct vector{
	uint32_t length_bytes;
	uint32_t length;
	struct vector* (*resize)(struct vector*, uint32_t);
	void (*destroy)(struct vector*);
	uint32_t data[];
} vector;

void* kmalloc(uint32_t length);
void kfree(void* address);
// void memcpy(void* start, uint32_t length, void* dest);


vector* resize_vector(vector* target_vector, uint32_t new_size){
	uint32_t req_bytes = (new_size*sizeof(uint32_t)) + sizeof(vector);
	vector* realloced_vector = kmalloc(req_bytes);
	if(realloced_vector == 0x0){
		return(0x0);
	}
	if(req_bytes > target_vector->length_bytes){
		//new vector is larger
		memcpy(realloced_vector, target_vector, target_vector->length_bytes);
		memset((void*)(((uint32_t)realloced_vector)+target_vector->length_bytes), 0x0, (req_bytes-(target_vector->length_bytes)));
	}else{
		memcpy(realloced_vector, target_vector, req_bytes);
	}
	kfree(target_vector);
	realloced_vector->length_bytes = req_bytes;
	realloced_vector->length = new_size;
	return(realloced_vector);
}

void destroy_vector(vector* target_vector){
	kfree(target_vector);
}

void vector_init(void){
	
}

vector* new_vector(void){
	vector* tmp = kmalloc(sizeof(vector));
	if(tmp ==0){
		return(0x0);
	}
	tmp->length_bytes = 0+sizeof(vector);
	tmp->length = 0;
	tmp->resize = resize_vector;
	tmp->destroy = destroy_vector;
	return(tmp);
}