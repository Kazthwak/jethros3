#ifndef STDLIB_H
#define STDLIB_H
int printf(const char* a,  ...);
int puts(const char *s);

/*
static inline int printf(const char* a,  ...){
}

static inline int puts(const char *s){
}
*/
#undef STDLIB_H
#endif