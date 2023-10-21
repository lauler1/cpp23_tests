# include <stdio.h>

/* Demo code showing the usage of the cleanup variable
   attribute. See:http://gcc.gnu.org/onlinedocs/gcc/Variable-Attributes.html
*/

/* cleanup function
   the argument is a int * to accept the address
   to the final value
*/

void clean_up(int *final_value)
{
  printf("  Cleaning up\n");
  printf("  Final value: %d\n",*final_value);

}

void func (){
  /* declare cleanup attribute along with initiliazation
     Without the cleanup attribute, this is equivalent 
     to:
     int avar = 1;
  */
puts(" func 1");  
  int avar __attribute__ ((__cleanup__(clean_up))) = 1;
puts(" func 2");
  avar = 5;
puts(" func 3");

}

int main(int argc, char **argv)
{
puts("main 1");

	func();

puts("main 2");
  return 0;
}
