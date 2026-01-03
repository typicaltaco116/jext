#include "types.h"

#include "data_structures.h"

void cursor_attach_buffer(line_t* buffer);

void move_cursor(int32_t row, int32_t column);

void get_cursor_pos(int32_t* row, int32_t* column);

line_t* get_cursor_line(void);
node_t* get_cursor_node(void);

void walk_cursor(int32_t deltaRow, int32_t deltaColumn);

void insert_value_on_cursor(char c);

void insert_line_on_cursor(void);

bool delete_value_before_cursor(void);

void delete_line_on_cursor(void);

void delete_cursor_line(void);
