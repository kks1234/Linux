#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>


int main()
{
   void* handler;
   char* error;
   void(*print_hello)(void);
   
   if( (handler = dlopen("./libttt.so", RTLD_LAZY)) == NULL) {
	 perror("dlopen error");
         exit(1);
   }
   
   print_hello = dlsym(handler, "print_hello");

   if( (error = dlerror()) != NULL ) {
	   perror("dlerror error");
	   exit(1);
   }

   print_hello();
   dlclose(handler);
      
   exit(0);
}
