#include "kernel.h"
#include "include.h"


void main(){
void* test = fl_fopen("/test.txt", "r");
hexdword(test); newline();
hexbyte(fl_fgetc(test));
}