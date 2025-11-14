#ifndef STRING_H_CODE
#define STRING_H_CODE

#define size_t_t 	long unsigned int
#define uint8_t_t	unsigned char


void* memcpy(void* dest, const void* src, size_t_t len){
	for(size_t_t i = 0; i < len; i++){
		((uint8_t_t*)dest)[i] = ((uint8_t_t*)src)[i];
	}
	return(dest);
}

void* memset(void* dest, int val, size_t_t len){
	for(size_t_t i = 0; i < len; i++){
		((uint8_t_t*)dest)[i] = (uint8_t_t)val;
	}
	return(dest);
}

int strncmp(const char* str1, const char* str2, size_t_t len){
	for(size_t_t i = 0; i < len; i++){
		unsigned char a = str1[i];
		unsigned char b = str2[i];
		if(a != b){
			return(a-b);
		}
		if(a == 0){return(0);}
	}
	return(0);
}

char* strncpy(char* dest, const char* src, size_t_t len){
	char going = 1;
	for(size_t_t i = 0; i < len; i++){
		if(!going){
			dest[i] = '\0';
			continue;
		}
		char a = src[i];
		dest[i] = a;
		if(a == '\0'){
			going = 0;
		}
	}
	return(dest);
}

size_t_t strlen(const char* string){
	size_t_t i = 0;
	while(1){
		if(string[i] == '\0'){return(i);}
		i++;
	}
}

#undef uint8_t_t
#undef size_t_t
#endif