#include "input.h"

#include "types.h"
#include "ANSI_codes.h"

#include <stdio.h>

char getKey(void)
{
  return (char)getc(stdin);
}
