

#include "syscall.h"

int
main()
{
  int result;
  
  result = Create("Hello.txt");

  Halt();
  /* not reached */
}
