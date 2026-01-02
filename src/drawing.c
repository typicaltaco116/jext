#include "drawing.h"

#include "types.h"
#include "terminal.h"
#include "ANSI_codes.h"
#include "data_structures.h"
#include "cursor.h"

#include <stdlib.h>
#include <stdio.h>

static int32_t _textWindowOrigin; // this is the text buffer row that is currently
                                  // at the top of the text window

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

void draw_to_end(node_t* node, int32_t row, int32_t column, int32_t maxColumns)
{
  ttyMoveCursor(column, row);
  
  while ((node != NULL) && (column < maxColumns)) {
    ttyPutChar(node->c);
    node = node->next;
    column++;
  }
}

void draw_line(line_t* line, int32_t row, int32_t maxColumns)
{
  draw_to_end(line->base, row, 0, maxColumns);
}

void draw_entire_text_window(line_t* startLine, int32_t winOrigin, int32_t* returnRowCount)
{
  line_t* line;
  int32_t row = 0;

  _textWindowOrigin = winOrigin;

  line = startLine;
  while ((line != NULL) && (row < get_text_window_rows())) {
    draw_line(line, row++, get_text_window_columns());
    line = line->next;
  }
  ttyRefresh();

  *returnRowCount = row;
}

void scroll_text_window(line_t* line, int32_t n)
// CURRENTLY ONLY WORKS WITH INCREMENTS OF 1
// -1 scrolls up
// +1 scrolls down
{
  ttyScroll(n);
  _textWindowOrigin += n;

  if (n > 0) { // scroll down
    draw_line(line, get_text_window_rows() - 1, get_text_window_columns());

  } else if (n < 0) { // scroll up
    draw_line(line, 0, get_text_window_columns());
  }
  ttyRefresh();
}

static void getCursorScreenPosition(int* row, int* column)
{
  get_cursor_pos(row, column);
  row -= _textWindowOrigin;
}

void draw_cursor(void)
{
  int bufferRow, bufferColumn;

  get_cursor_pos(&bufferRow, &bufferColumn);
  if (bufferRow >= get_text_window_rows() + _textWindowOrigin) {
    scroll_text_window(get_cursor_line(), SCROLL_DOWN);
  } else if (bufferRow < _textWindowOrigin) {
    scroll_text_window(get_cursor_line(), SCROLL_UP);
  }

  ttyMoveCursor(bufferColumn, bufferRow - _textWindowOrigin);
}

void draw_insert_text(char c, int32_t maxColumn)
{
  int screenRow, screenColumn;

  getCursorScreenPosition(&screenRow, &screenColumn);

  insert_value_on_cursor(c);
  draw_line(get_cursor_line(), screenRow, maxColumn);
  draw_cursor();
  ttyRefresh();
}

void draw_delete_text(int32_t maxColumn)
{
  int screenRow, screenColumn;

  getCursorScreenPosition(&screenRow, &screenColumn);

  delete_value_before_cursor();
  ttyDeleteTillLineEnd();
  draw_line(get_cursor_line(), screenRow, maxColumn);
  draw_cursor();
  ttyRefresh();
}
