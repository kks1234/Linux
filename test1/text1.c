extern int A;

long my_pow(int numb)
{
   long num = 1;
   int i;
   for(i=0; i<10; i++)
   {
      num *= A;
   }
   return num;
}







