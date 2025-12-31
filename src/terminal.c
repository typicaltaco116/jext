#include "terminal.h"

#include "ANSI_codes.h"
#include "types.h"

#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ncurses.h>

static int32_t _windowXSize = 0;
static int32_t _windowYSize = 0;

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

  nodelay(stdscr, TRUE);

  getTerminalSize(&_windowXSize, &_windowYSize); // fill variables

  newlineClear(_windowYSize);
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
  char buffer[20];
  snprintf(buffer, 20, ANSI_ESC "[%u;%uH", y, x); // kinda slow consider changing
  fputs(buffer, stdout);
  fflush(stdout);
}

int32_t ttyGetWindowXSize(void)
{
  return _windowXSize;
}

int32_t ttyGetWindowYSize(void)
{
  return _windowYSize;
}
