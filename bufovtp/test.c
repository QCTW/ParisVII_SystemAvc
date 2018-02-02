char sc[] ="Insert your code here";

int main(int argc, char **argv) 
{ 
  int (*shellcode)() = (int (*)())sc; 
  shellcode(); 
  return 0; 
}

