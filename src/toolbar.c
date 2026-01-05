#include "toolbar.h"

#include "types.h"
#include "terminal.h"

#include <ncurses.h>
#include <string.h>

#define TOOLBAR_FILENAME_ROW 0

static void winPutStr(WINDOW*, const char*);

static WINDOW* _toolbarWindow;

void toolbar_init(void)
{
  _toolbarWindow = newwin(TOOLBAR_WINDOW_ROWS, get_terminal_columns(), get_text_window_rows(), 0);
  wrefresh(_toolbarWindow);
}

void toolbar_update_filename(const char* filename)
{
  wattr_on(_toolbarWindow, WA_STANDOUT | WA_BOLD, NULL);

  wmove(_toolbarWindow, TOOLBAR_FILENAME_ROW, get_terminal_columns() / 2 - (strlen(filename) + 8) / 2);
  winPutStr(_toolbarWindow, "--- ");
  winPutStr(_toolbarWindow, filename);
  winPutStr(_toolbarWindow, " ---");

  wattr_off(_toolbarWindow, WA_STANDOUT | WA_BOLD, NULL);
  wrefresh(_toolbarWindow);
}

static void winPutStr(WINDOW* window, const char* str)
{
  while (*str) {
    waddch(window, *str++);
  }
}
