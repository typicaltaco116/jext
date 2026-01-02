#include "types.h"

#include "data_structures.h"

typedef enum {
  UNIX_LINE_END,
  DOS_LINE_END,
  OSX_LINE_END,
} line_end_e;

line_t* create_file_buffer(const char* filename, line_end_e* newlineMode);

bool write_file_buffer(line_t* buffer, const char* filename, line_end_e newlineMode);
