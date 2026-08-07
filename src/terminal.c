#include <jext/terminal.h>

#include <jext/ANSI_codes.h>
#include <jext/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <ncurses/ncurses.h>

#ifdef _WIN32
#include <windows.h>
#endif

static int32_t _terminalColumns = 0;
static int32_t _terminalRows = 0;
static int32_t _textColumns = 0;
static int32_t _textRows = 0;

static WINDOW* _textWindow;

#ifdef _WIN32
static DWORD _originalConsoleMode;
#endif

static void getTerminalSize(int32_t* x, int32_t* y)
{
    getmaxyx(stdscr, *y, *x);
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

#ifdef _WIN32
    HANDLE stdHandle = GetStdHandle(STD_INPUT_HANDLE);
    if (GetConsoleMode(stdHandle, &_originalConsoleMode)) {
        DWORD mode = _originalConsoleMode;
        // Disable line buffer, echo, and system processing of signals
        mode &= ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT | ENABLE_PROCESSED_INPUT);
        mode |= ENABLE_VIRTUAL_TERMINAL_INPUT;
        SetConsoleMode(stdHandle, mode);
    }
    // Disable C runtime buffering so fgetc grabs keys instantly
    setvbuf(stdin, NULL, _IONBF, 0);
#endif
}

void ttyRestore(void)
{
    endwin();
    ttySetCursorVisibility(1);
#ifdef _WIN32
    HANDLE stdHandle = GetStdHandle(STD_INPUT_HANDLE);
    SetConsoleMode(stdHandle, _originalConsoleMode);
#endif
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
