#include <stdio.h>


int main()
{
   int a[] = {1,2,3,4,5,6};
   printf("%ld",sizeof(a)/sizeof(a[0]));
   return 0;
}
