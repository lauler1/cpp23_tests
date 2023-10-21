
This is not actually a C++ example, this is a C test for automatic cleanup attribute from clang and gcc: `__cleanup__`.

See: http://echorand.me/site/notes/articles/c_cleanup/cleanup_attribute_c.html

C:
```
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
  printf("Cleaning up\n");
  printf("Final value: %d\n",*final_value);

}

int main(int argc, char **argv)
{
  /* declare cleanup attribute along with initiliazation
     Without the cleanup attribute, this is equivalent 
     to:
     int avar = 1;
  */
  
  int avar __attribute__ ((__cleanup__(clean_up))) = 1;
  avar = 5;

  return 0;
}
```


```bash
$ gcc -Wall cleanup_attribute_demo.c
$ ./a.out
Cleaning up
Final value: 5
```
