// A compiler avec -fno-stack-protector a partir de gcc 4.1 
#include <string.h> 
void foo(const char* buf) 
{ 
  char buffer[100]; 
  strcpy(buffer, buf);
} 

int main(int argc, char *argv[]) 
{ 
  if(argc > 1) 
    foo(argv[1]); 
  printf("Done\n");
  return 0; 
}
