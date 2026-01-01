#include "input.h"

#include "types.h"
#include "ANSI_codes.h"

#include <stdio.h>

input_type_e getInput(int* returnData)
{
  int c;

  c = fgetc(stdin);
  if (c != (int)ANSI_ESC_CHAR) { // plain text
    *returnData = c;
    return INPUT_TEXT;
  }

  c = fgetc(stdin);
  if (c == '[') { // navigation keys
    c = fgetc(stdin);
    switch (c) {
      case 'A':
        *returnData = DIR_UP;
        break;
      case 'B':
        *returnData = DIR_DOWN;
        break;
      case 'D':
        *returnData = DIR_LEFT;
        break;
      case 'C':
        *returnData = DIR_RIGHT;
        break;
      default:
        return INPUT_ERROR;
    }
    return INPUT_ARROW;
  }

  return INPUT_ERROR;
}
