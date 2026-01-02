#include "input.h"

#include "types.h"
#include "ANSI_codes.h"
#include "cursor.h"
#include "drawing.h"

#include <stdio.h>

static input_type_e handleEscapeCode(int*);
static void inputHandleArrow(dir_e);
static void inputHandleText(char);
static void inputHandleControl(char);

input_type_e getInput(int* returnData)
{
  int c;

  c = fgetc(stdin);

  switch (c) {
    case (int)ANSI_ESC_CHAR:
      return handleEscapeCode(returnData);

    case (int)'\r': // carriage return from return key
    case (int)'\x7F': // delete character
      *returnData = c;
      return INPUT_CONTROL;

    case ' ' ... '~':
      *returnData = c;
      return INPUT_TEXT;


    default:
      return INPUT_ERROR;
  }
}

static input_type_e handleEscapeCode(int* returnData)
{
  if (fgetc(stdin) != (int)'[') {
    return INPUT_ERROR;
  }

  switch (fgetc(stdin)) {
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

void input_handler(void)
{
  int inputData;

  switch (getInput(&inputData)) {
    case INPUT_ARROW:
      inputHandleArrow((dir_e)inputData);
      break;

    case INPUT_TEXT:
      inputHandleText((char)inputData);
      break;

    case INPUT_CONTROL:
      inputHandleControl((char)inputData);
      break;

    default:
  }
}

static void inputHandleArrow(dir_e arrow)
{
  switch (arrow) {
    case DIR_UP:
      walk_cursor(-1, 0);
      break;

    case DIR_DOWN:
      walk_cursor(1, 0);
      break;

    case DIR_LEFT:
      walk_cursor(0, -1);
      break;

    case DIR_RIGHT:
      walk_cursor(0, 1);
      break;

    default:
  }

  draw_cursor();
}

static void inputHandleText(char c)
{
  draw_insert_text(c, 80);
}

static void inputHandleControl(char c)
{
  switch (c) {
    case ANSI_DEL_CHAR:
      draw_delete_text(80);
      break;

    default:
  }
}
