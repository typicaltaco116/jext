#include "types.h"
#include "data_structures.h"

typedef enum{
  TTY_RED,
  TTY_GREEN,
  TTY_BLUE,
  TTY_YELLOW,
  TTY_MAGENTA,
  TTY_CYAN,
  TTY_DEFAULT,
} tty_colors_e;

void setDrawColor(tty_colors_e color);

void draw_line(line_t* line, int32_t row, int32_t maxColumns);

void draw_entire_buffer(line_t* buffer);
