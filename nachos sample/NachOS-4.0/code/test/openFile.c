

#include "syscall.h"

int
main()
{
  int result;
  
  result = Open("Hello.txt");

  Halt();
  /* not reached */
}
