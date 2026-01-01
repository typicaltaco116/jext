#include "drawing.h"

#include "types.h"
#include "terminal.h"
#include "ANSI_codes.h"
#include "data_structures.h"
#include "cursor.h"

#include <stdlib.h>
#include <stdio.h>

void setDrawColor(tty_colors_e color)
{
  switch (color) {
    case TTY_RED:
      fputs(ANSI_COLOR_RED, stdout);
      break;

    case TTY_GREEN:
      fputs(ANSI_COLOR_GREEN, stdout);
      break;

    case TTY_BLUE:
      fputs(ANSI_COLOR_BLUE, stdout);
      break;

    case TTY_YELLOW:
      fputs(ANSI_COLOR_YELLOW, stdout);
      break;

    case TTY_MAGENTA:
      fputs(ANSI_COLOR_MAGENTA, stdout);
      break;

    case TTY_CYAN:
      fputs(ANSI_COLOR_CYAN, stdout);
      break;

    case TTY_DEFAULT:
      fputs(ANSI_COLOR_DEFAULT, stdout);
      break;
  }
  fflush(stdout);
}

void draw_line(line_t* line, int32_t row, int32_t maxColumns)
{
  node_t* node;
  int32_t column = 0;

  node = line->base;
  ttyMoveCursor(0, row);
  
  while ((node != NULL) && (column < maxColumns)) {
    fputc(node->c, stdout);
    node = node->next;
    column++;
  }

  fflush(stdout);
}

void draw_entire_buffer(line_t* buffer)
{
  line_t* line;
  int32_t row = 1;

  line = buffer;
  while ((line != NULL) && (row < ttyGetWindowYSize())) {
    draw_line(line, row++, ttyGetWindowXSize());
    line = line->next;
  }
}

void draw_cursor(void)
{
  int x, y;

  get_cursor_pos(&x, &y);
  ttyMoveCursor(x, y);
}
