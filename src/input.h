#include "types.h"

typedef enum {
  INPUT_ERROR,
  INPUT_TEXT,
  INPUT_ARROW,
  INPUT_CONTROL,
} input_type_e;

typedef enum {
  DIR_UP,
  DIR_DOWN,
  DIR_LEFT,
  DIR_RIGHT,
} dir_e;

input_type_e getInput(int* returnData);

void input_handler(void);
