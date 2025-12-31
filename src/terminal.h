#include "types.h"

#define CENTERED_LINE_SIZE 40

void ttySetup(void);

void ttyRestore(void);

void ttyMoveCursor(u16 x, u16 y);

int32_t ttyGetWindowXSize(void);
int32_t ttyGetWindowYSize(void);
