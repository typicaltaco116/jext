#pragma once

#define ANSI_ESC "\x1B"

#define ANSI_ESC_CHAR     '\x1B'
#define ANSI_DEL_CHAR     '\x7F'
#define ANSI_BS_CHAR      '\b'

#define ANSI_CTRL_X_CHAR  '\x18'
#define ANSI_CTRL_T_CHAR  '\x14'
#define ANSI_CTRL_S_CHAR  '\x13'

#define ANSI_CLEAR  ANSI_ESC "[2J"
#define ANSI_HOME   ANSI_ESC "[H"
#define ANSI_DEL    ANSI_ESC "[3~"

#define ANSI_UP     ANSI_ESC "[A"
#define ANSI_DOWN   ANSI_ESC "[B"
#define ANSI_LEFT   ANSI_ESC "[D"
#define ANSI_RIGHT  ANSI_ESC "[C"

#define ANSI_MOVE_CURSOR(x,y) (ANSI_ESC "[" y ";" x "H")

#define ANSI_COLOR_RED       ANSI_ESC "[31m"
#define ANSI_COLOR_GREEN     ANSI_ESC "[32m"
#define ANSI_COLOR_BLUE      ANSI_ESC "[34m"
#define ANSI_COLOR_YELLOW    ANSI_ESC "[33m"
#define ANSI_COLOR_MAGENTA   ANSI_ESC "[35m"
#define ANSI_COLOR_CYAN      ANSI_ESC "[36m"
#define ANSI_COLOR_DEFAULT   ANSI_ESC "[0m"

#define ANSI_INVERT_ON       ANSI_ESC "[7m"
