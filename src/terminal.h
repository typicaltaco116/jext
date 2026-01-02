#include "types.h"

#define CENTERED_LINE_SIZE 40

void ttySetup(void);

void ttyRestore(void);

void ttyMoveCursor(u16 x, u16 y);
void ttyPutChar(char c);
void ttyInsertChar(char c);
void ttyDeleteChar(void);
void ttyDeleteLine(void);
void ttyDeleteTillLineEnd(void);
void ttyInsertLine(void);

int32_t get_terminal_columns(void);
int32_t get_terminal_rows(void);
int32_t get_text_window_rows(void);
int32_t get_text_window_columns(void);

void ttyScroll(int32_t n);

void ttyRefresh(void);
