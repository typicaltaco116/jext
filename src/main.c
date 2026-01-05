#include "drawing.h"
#include "data_structures.h"
#include "file_handler.h"
#include "terminal.h"
#include "input.h"
#include "cursor.h"
#include "toolbar.h"

#include <stdlib.h>
#include <signal.h>
#include <stdio.h>

line_t* fileBuffer;
char* filenameArgument;
line_end_e lineEndMode;

static void program_interrupt_handler(int);
static void program_argument_handler(int, char**);

static void loadFileBuffer(void)
{
  fileBuffer = create_file_buffer(filenameArgument, &lineEndMode);
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
  toolbar_update_filename(filenameArgument);
  loadFileBuffer();
  initTextWindow();

  while(input_handler());

  ttyRestore();

  if(!write_current_buffer(filenameArgument)) {
    printf("Failed to write file %s\n", filenameArgument);
  } else {
    printf("Successful write to file %s\n", filenameArgument);
  }

  free_all_lines(&fileBuffer);

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
  filenameArgument = argv[1];
}
