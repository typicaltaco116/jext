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

typedef enum {
  SM_INSERT,
  SM_EDIT_FILENAME,
  SM_QUIT,
} sm_modes_e;


bool input_handler(void);
