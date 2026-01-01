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
    ttyPutChar(node->c);
    node = node->next;
    column++;
  }

  ttyRefresh();
}

void draw_entire_text_window(line_t* startLine, int32_t* returnRowCount)
{
  line_t* line;
  int32_t row = 0;

  line = startLine;
  while ((line != NULL) && (row < get_text_window_rows())) {
    draw_line(line, row++, get_text_window_columns());
    line = line->next;
  }

  *returnRowCount = row;
}

void scroll_text_window(line_t* line, int32_t n)
// CURRENTLY ONLY WORKS WITH INCREMENTS OF 1
// -1 scrolls up
// +1 scrolls down
{
  ttyScroll(n);
  if (n > 0) { // scroll down
    draw_line(line, get_text_window_rows() - 1, get_text_window_columns());

  } else if (n < 0) { // scroll up
    draw_line(line, 0, get_text_window_columns());
  }
}

void draw_cursor(void)
{
  int x, y;

  get_cursor_pos(&y, &x);
  ttyMoveCursor(x, y);
}
