

#include "syscall.h"

int main()
{
  int result;
  
  result = Open("hello.txt");
  Halt();
  /* not reached */
}