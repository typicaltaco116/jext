#include "drawing.h"
#include "data_structures.h"
#include "file_handler.h"
#include "terminal.h"
#include "input.h"
#include "cursor.h"

#include <stdlib.h>
#include <signal.h>
#include <stdio.h>

line_t* fileBuffer;
char* filenameGlobal;
line_end_e lineEndModeGlobal;

static void program_interrupt_handler(int);
static void program_argument_handler(int, char**);
static void programInit(void);
static void programExit(int);

static void textEdit(char* filename)
{
  filenameGlobal = filename;

  fileBuffer = create_file_buffer(filename, &lineEndModeGlobal);

  draw_entire_text_window(fileBuffer, 0);

  cursor_attach_buffer(fileBuffer);
  move_cursor(0, 0);
  draw_cursor();
  ttyRefresh();

  while (input_handler());
}

int main(int argc, char** argv)
{
  program_argument_handler(argc, argv);
  programInit();

  textEdit(argv[1]);

  programExit(0);
}

static void program_interrupt_handler(int signal)
{
  programExit(0);
}

static void program_argument_handler(int argc, char** argv)
{
  if (argc != 2) {
    printf("Error: INCORRECT ARGUMENTS\n");
    exit(1);
  }
}

static void programInit(void)
{
  ttySetup();

  signal(SIGINT, program_interrupt_handler);
}

static void programExit(int code)
{
  ttyRestore();

  if(!write_file_buffer(fileBuffer, filenameGlobal, lineEndModeGlobal)) {
    printf("Failed to write file %s\n", filenameGlobal);
  } else {
    printf("Successful write to file %s\n", filenameGlobal);
  }

  free_all_lines(&fileBuffer);
  exit(code);
}
