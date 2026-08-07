#include "types.h"

void toolbar_init(void);

void toolbar_update_filename(const char* filename);

void draw_toolbar(bool blinkEnabled);

void toolbar_append_filename_char(char c);

void toolbar_remove_filename_char(void);

void toolbar_free_all(void);

char* get_toolbar_filename_string(void);
