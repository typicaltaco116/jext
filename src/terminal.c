#include "terminal.h"

#include "ANSI_codes.h"
#include "types.h"

#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ncurses.h>

static int32_t _terminalColumns = 0;
static int32_t _terminalRows = 0;
static int32_t _textColumns = 0;
static int32_t _textRows = 0;

static WINDOW* _textWindow;

static void getTerminalSize(int32_t* x, int32_t* y)
{
  struct winsize w;

  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
    exit(EXIT_FAILURE);
  }

  *x = w.ws_col;
  *y = w.ws_row;
}

void ttySetup(void)
{
  initscr(); // init ncurses screen for restoring later
  cbreak(); // canonical mode
  noecho(); // disable local echo

  getTerminalSize(&_terminalColumns, &_terminalRows); // fill variables

  _textRows = _terminalRows - TOOLBAR_WINDOW_ROWS;
  _textColumns = _terminalColumns;
  _textWindow = newwin(_textRows, _textColumns, 0, 0);
  scrollok(_textWindow, true);
}

void ttyRestore(void)
{
  endwin();
  ttySetCursorVisibility(1);
}

void ttyMoveCursor(u16 x, u16 y)
{
  /*
  char buffer[20];
  snprintf(buffer, 20, ANSI_ESC "[%u;%uH", y, x); // kinda slow consider changing
  fputs(buffer, stdout);
  fflush(stdout);
  */
  wmove(_textWindow, y, x);
}

void ttyPutChar(char c)
{
  waddch(_textWindow, c);
}

void ttyInsertChar(char c)
{
  winsch(_textWindow, c);
}

void ttyDeleteChar(void)
{
  wdelch(_textWindow);
}

void ttyDeleteLine(void)
{
  wdeleteln(_textWindow);
}

void ttyDeleteTillLineEnd(void)
{
  wclrtoeol(_textWindow);
}

void ttyInsertLine(void)
{
  winsertln(_textWindow);
}

void ttyRefresh(void)
{
  wrefresh(_textWindow);
}

void ttySetCursorVisibility(bool enabled)
{
  if (enabled) {
    fputs(ANSI_CURSOR_SHOW, stdout);
  } else {
    fputs(ANSI_CURSOR_HIDE, stdout);
  }
  fflush(stdout);
}

int32_t get_terminal_columns(void)
{
  return _terminalColumns;
}

int32_t get_terminal_rows(void)
{
  return _terminalRows;
}

int32_t get_text_window_rows(void)
{
  return _textRows;
}

int32_t get_text_window_columns(void)
{
  return _textColumns;
}

void ttyScroll(int32_t n)
{
  wscrl(_textWindow, n);
}
