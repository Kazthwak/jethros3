#ifndef STDLIB_H_CODE
#define STDLIB_H_CODE
int printf(const char* a,  ...);
int puts(const char *s);


int printf(const char* a,  ...){
(void)a;
return(0);
}

int puts(const char *s){
	(void)s ;
	return(0);
}

#undef STDLIB_H_CODE
#endif