#include "types.h"
#include "data_structures.h"

#define SCROLL_DOWN (1)
#define SCROLL_UP   (-1)

typedef enum {
  TTY_RED,
  TTY_GREEN,
  TTY_BLUE,
  TTY_YELLOW,
  TTY_MAGENTA,
  TTY_CYAN,
  TTY_DEFAULT,
} tty_colors_e;

void setDrawColor(tty_colors_e color);

void draw_to_end(node_t* node, int32_t row, int32_t column, int32_t maxColumns);
void draw_line(line_t* line, int32_t row, int32_t maxColumns);
void draw_entire_text_window(line_t* startLine, int32_t winOrigin);

void scroll_text_window(line_t* line, int32_t n);

void draw_cursor(void);
void draw_insert_text(char c, int32_t maxColumn);
void draw_delete_text(int32_t maxColumn);
void draw_insert_newline(int32_t maxColumn);
