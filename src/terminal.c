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

static void newlineClear(u16 totalRows)
{
  for (u16 i = totalRows; i != 0; i--) {
    fputs("\r\n", stdout);
  }
  fflush(stdout);
}

void ttySetup(void)
{
  initscr(); // init ncurses screen for restoring later
  cbreak(); // canonical mode
  noecho(); // disable local echo

  getTerminalSize(&_terminalColumns, &_terminalRows); // fill variables

  _textRows = _terminalRows;
  _textColumns = _terminalColumns;
  _textWindow = newwin(_textRows, _textColumns, 0, 0); // create new window that is the whole terminal size
  scrollok(_textWindow, true);
  newlineClear(_terminalRows);
}

void ttyRestore(void)
{
  endwin();
  fputs(ANSI_HOME, stdout);
  fputs(ANSI_CLEAR, stdout);
  fflush(stdout);
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
  ttyRefresh();
}

void ttyPutChar(char c)
{
  if (c == '\0') {
    return;
  }
  waddch(_textWindow, c);
}

void ttyDeleteLine(void)
{
  wdeleteln(_textWindow);
}

void ttyDeleteTillLineEnd(void)
{
  wclrtoeol(_textWindow);
}

void ttyRefresh(void)
{
  wrefresh(_textWindow);
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
