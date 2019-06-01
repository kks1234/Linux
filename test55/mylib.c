#include <stdio.h>
#include "mylib.h"


void display(const char* buf)
{
    if(buf != NULL) {
	printf("%s",buf);
    }
}


void print_hello()
{
   printf("Hello World\n");
}


