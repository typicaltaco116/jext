#include "input.h"

#include "types.h"
#include "ANSI_codes.h"
#include "cursor.h"
#include "drawing.h"
#include "terminal.h"
#include "toolbar.h"

#include <stdio.h>

bool _exitFlag = false;
sm_modes_e _mode = SM_INSERT;

static input_type_e getInput(int*);
static void inputHandleArrow(dir_e);
static void inputHandleText(char);
static void inputHandleControl(char);

bool input_handler(void)
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

  return !_exitFlag;
}


static input_type_e handleEscapeCode(int*);

static input_type_e getInput(int* returnData)
{
  int c;

  c = fgetc(stdin);

  switch (c) {
    case (int)ANSI_ESC_CHAR:
      return handleEscapeCode(returnData);

    case ' ' ... '~':
      *returnData = c;
      return INPUT_TEXT;


    default:
      *returnData = c;
      return INPUT_CONTROL; // assume everything else is control
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

static void insertModeArrowHandler(dir_e);

static void inputHandleArrow(dir_e arrow)
{
  switch (_mode) {
    case SM_INSERT:
    insertModeArrowHandler(arrow);
    break;

    case SM_EDIT_FILENAME:
    break;

    default:
  }
}

static void insertModeArrowHandler(dir_e arrow)
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
  ttyRefresh();
}

static void inputHandleText(char c)
{
  switch (_mode) {
    case SM_INSERT:
    draw_insert_text(c, get_terminal_columns());
    ttyRefresh();
    break;

    case SM_EDIT_FILENAME:
    toolbar_append_filename_char(c);
    draw_toolbar(true);
    break;

    default:
  }
}

static void insertModeControlHandler(char c);
static void editFilenameModeControlHandler(char c);

static void inputHandleControl(char c)
{
  switch (_mode) {
    case SM_INSERT:
    insertModeControlHandler(c);
    break;

    case SM_EDIT_FILENAME:
    editFilenameModeControlHandler(c);
    break;

    default:
  }
}

static void insertModeControlHandler(char c)
{
  switch (c) {
    case ANSI_DEL_CHAR:
      draw_delete_text(get_terminal_columns());
      ttyRefresh();
      break;

    case '\r':
      draw_insert_newline(get_terminal_columns());
      ttyRefresh();
      break;

    case ANSI_CTRL_X_CHAR:
      _exitFlag = true;
      break;

    case ANSI_CTRL_T_CHAR:
      ttySetCursorVisibility(0);
      draw_toolbar(true);
      _mode = SM_EDIT_FILENAME;
      break;

    case ANSI_CTRL_S_CHAR:
      break;

    default:
  }
}

static void editFilenameModeControlHandler(char c)
{
  switch (c) {
    case ANSI_DEL_CHAR:
      toolbar_remove_filename_char();
      draw_toolbar(true);
      break;

    case '\r':
    case ANSI_CTRL_T_CHAR:
      _mode = SM_INSERT;
      draw_toolbar(false);
      ttySetCursorVisibility(1);
      draw_cursor();
      ttyRefresh();
      break;

    case ANSI_CTRL_X_CHAR:
      _exitFlag = true;
      break;

    case ANSI_CTRL_S_CHAR:
      break;

    default:
  }
}
