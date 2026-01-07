#include "toolbar.h"

#include "types.h"
#include "terminal.h"

#include <ncurses.h>
#include <stdlib.h>

#define TOOLBAR_FILENAME_ROW 0
#define FILENAMESTRING_EXTRA_SPACE 6

//static char* _filenameString;
//static int32_t _filenameStringUsed;
//static int32_t _filenameStringLength; // includes null character

typedef struct {
  char* ptr;
  int32_t used;
  int32_t length; // includes NULL character
} dynamicString_t;

static dynamicString_t _filenameString;

static void winPutStr(WINDOW*, const char*);
static void filenameStringInit(void);
static void filenameStringAddStr(const char* str);

static WINDOW* _toolbarWindow;

void toolbar_init(void)
{
  _toolbarWindow = newwin(TOOLBAR_WINDOW_ROWS, get_terminal_columns(), get_text_window_rows(), 0);
  wrefresh(_toolbarWindow);
}

 void draw_toolbar(bool blinkEnabled)
{
  wmove(_toolbarWindow, TOOLBAR_FILENAME_ROW, 0);
  wclrtoeol(_toolbarWindow);

  wattr_on(_toolbarWindow, WA_STANDOUT | WA_BOLD, NULL);

  wmove(_toolbarWindow, TOOLBAR_FILENAME_ROW, get_terminal_columns() / 2 - (_filenameString.used + 7) / 2);

  if (blinkEnabled) {
    wattr_on(_toolbarWindow, WA_BLINK, NULL);
    winPutStr(_toolbarWindow, "--- ");
    wattr_off(_toolbarWindow, WA_BLINK, NULL);
  } else {
    winPutStr(_toolbarWindow, "--- ");
  }

  winPutStr(_toolbarWindow, _filenameString.ptr);

  if (blinkEnabled) {
    wattr_on(_toolbarWindow, WA_BLINK, NULL);
    winPutStr(_toolbarWindow, " ---");
    wattr_off(_toolbarWindow, WA_BLINK, NULL);
  } else {
    winPutStr(_toolbarWindow, " ---");
  }

  wattr_off(_toolbarWindow, WA_STANDOUT | WA_BOLD, NULL);
  wrefresh(_toolbarWindow);
}

void toolbar_update_filename(const char* filename)
{
  filenameStringInit();
  filenameStringAddStr(filename);

  draw_toolbar(false);
}

static void winPutStr(WINDOW* window, const char* str)
{
  while (*str) {
    waddch(window, *str++);
  }
}

static void filenameStringInit(void)
{
  _filenameString.ptr = malloc(FILENAMESTRING_EXTRA_SPACE * sizeof(char));
  _filenameString.length = FILENAMESTRING_EXTRA_SPACE * sizeof(char);
  _filenameString.used = 1;
  _filenameString.ptr[0] = '\0';
}

static void filenameStringAddChar(char c)
{
  _filenameString.used++;

  if (_filenameString.used <= _filenameString.length) {
    _filenameString.ptr = realloc(_filenameString.ptr,
      _filenameString.length + FILENAMESTRING_EXTRA_SPACE * sizeof(char));

    _filenameString.length += FILENAMESTRING_EXTRA_SPACE * sizeof(char);
  }

  _filenameString.ptr[_filenameString.used - 2] = c;
  _filenameString.ptr[_filenameString.used - 1] = '\0';
}

static void filenameStringAddStr(const char* str)
{
  while (*str) {
    filenameStringAddChar(*str++);
  }
}

void toolbar_append_filename_char(char c)
{
  filenameStringAddChar(c);
}

void toolbar_remove_filename_char(void)
{
  if (_filenameString.used == 1) {
    return;
  }

  _filenameString.used--;

  _filenameString.ptr[_filenameString.used - 1] = '\0';
}

void toolbar_free_all(void)
{
  free(_filenameString.ptr);
}

char* get_toolbar_filename_string(void)
{
  return _filenameString.ptr;
}
