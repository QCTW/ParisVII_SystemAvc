#include <stdio.h>
#include <string.h>

void foo(char* argv1);
 
void bar(void) 
{ 
  printf("Hacking Attempt.\n"); 
} 

int main(int argc, char *argv[]) 
{ 
  foo(argv[1]);
  return 0; 
}

void foo(char * arg)
{
  int i;
  char buffer[100]; 
  strcpy(buffer, arg);
  i = 0;
  printf("Quit 0x%p\n", buffer); 

}
