#include "drawing.h"
#include "data_structures.h"
#include "file_handler.h"
#include "terminal.h"
#include "input.h"
#include "cursor.h"
#include "toolbar.h"
#include "toolbar.h"

#include <stdlib.h>
#include <signal.h>
#include <stdio.h>

line_t* fileBuffer;

static void program_interrupt_handler(int);
static void program_argument_handler(int, char**);

static void loadFileBuffer(char* filename)
{
  fileBuffer = create_file_buffer(filename, NULL);
  if (fileBuffer == NULL) {
    fileBuffer = create_empty_line(); // new file
  }
  cursor_attach_buffer(fileBuffer);
}

static void initTextWindow(void)
{
  draw_entire_text_window(fileBuffer, 0);
  move_cursor(0, 0);
  draw_cursor();
  ttyRefresh();
}

int main(int argc, char** argv)
{
  program_argument_handler(argc, argv);
  signal(SIGINT, program_interrupt_handler);

  ttySetup();

  toolbar_init();
  toolbar_update_filename(argv[1]);
  loadFileBuffer(argv[1]);
  initTextWindow();

  while(input_handler());

  ttyRestore();

  if(!write_current_buffer(get_toolbar_filename_string())) {
    printf("Failed to write file %s\n", get_toolbar_filename_string());
  } else {
    printf("Successful write to file %s\n", get_toolbar_filename_string());
  }

  free_all_lines(&fileBuffer);
  toolbar_free_all();

  return 0;
}

static void program_interrupt_handler(int signal)
{
  ttyRestore();
  exit(0);
}

static void program_argument_handler(int argc, char** argv)
{
  if (argc != 2) {
    printf("Error: INCORRECT ARGUMENTS\n");
    exit(1);
  }
}
