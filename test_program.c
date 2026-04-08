#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
	int tmp = 0;
	tmp = fork();
	printf("TEST\n");
	printf("%d\n", tmp);
	if(tmp != 0){
		printf("THIS is THE parent PROCESS\n");
	}else{
		printf("THIS is THE child PROCESS\n"); 
	}
	while(1);
	return(0);
}