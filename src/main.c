#include "drawing.h"
#include "data_structures.h"
#include "file_handler.h"
#include "terminal.h"

#include <stdlib.h>
#include <signal.h>
#include <stdio.h>

void program_interrupt_handler(int);

static void testingFunc(const char* filename)
{
  line_t* fileBuffer;
  line_t* line;
  line_end_e lineEndingMode;
  int32_t row = 1;

  fileBuffer = create_file_buffer(filename, &lineEndingMode);

  line = fileBuffer; // set to initial first line

  while ((line != NULL) && (row < ttyGetWindowYSize())) {
    draw_line(line, row++, ttyGetWindowXSize());
    line = line->next;
  }

  free_all_lines(&fileBuffer);
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
  exit(code);
}

int main(int argc, char** argv)
{
  program_argument_handler(argc, argv);
  programInit();

  testingFunc(argv[1]);

  fgetc(stdin);

  programExit(0);
}

void program_interrupt_handler(int signal)
{
  programExit(0);
}
