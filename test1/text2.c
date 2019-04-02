#include <stdio.h>

extern long my_pow(int numb);

int A = 2;


int main()
{
  int n = 10;
  printf("2^10 = %ld\n",my_pow(n));
  return 0;

}

