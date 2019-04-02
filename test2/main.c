#include <stdio.h>

int get_big(int a, int b)
{
   return (a>b) ? a : b ;
}

int get_small(int a, int b)
{
   return (a<b) ? a : b;
}

int get_numb(int a, int b, int (*p)(int, int))
{
  return (*p)(a,b);
}

int main()
{
  int a,b;
  puts("Please input two numbers");
  scanf("%d%d",&a,&b);

  printf("%d\n",get_numb(a,b,get_small));
  
  return 0;

}

