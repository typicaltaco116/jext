#include "types.h"

#include "data_structures.h"

void cursor_attach_buffer(line_t* buffer)

void move_cursor(int32_t row, int32_t column)

void get_cursor_pos(int32_t* row, int32_t* column)

void walk_cursor(int32_t deltaRow, int32_t deltaColumn)

void insert_value_after_cursor(char c)

void delete_value_under_cursor(void)
