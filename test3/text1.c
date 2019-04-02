#include <stdio.h>

int get_num(int n)
{
#if 0
  int num = 0;
  while(num+= n,n--,n != 0);
  return num;
 #else
int sum = 0,i;
for(i=0; i<n; i++)
{
   sum += i;
}

return sum;



#endif
}



int main()
{
  int i = 100;
  int num;

  num = get_num(i);

  printf("1+2+3+4+...+%d = %d\n",i,num);

  return 0;

}
