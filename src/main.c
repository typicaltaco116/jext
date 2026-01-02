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

void program_interrupt_handler(int);

static void testingFunc(const char* filename)
{
  line_end_e lineEndingMode;
  int32_t rowsCount;

  fileBuffer = create_file_buffer(filename, &lineEndingMode);

  draw_entire_text_window(fileBuffer, 0, &rowsCount);

  cursor_attach_buffer(fileBuffer);
  move_cursor(0, 0);
  draw_cursor();

  while (1) { // currently need to quit with ctrl+c sry
    input_handler();
  }
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
  free_all_lines(&fileBuffer);
  ttyRestore();
  exit(code);
}

int main(int argc, char** argv)
{
  program_argument_handler(argc, argv);
  programInit();

  testingFunc(argv[1]);

  programExit(0);
}

void program_interrupt_handler(int signal)
{
  programExit(0);
}
