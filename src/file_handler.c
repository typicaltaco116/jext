#include "file_handler.h"

#include "types.h"
#include "data_structures.h"

#include <stdio.h>

#define LINE_MAX 150

static line_end_e getFileLineEnding(FILE*);
static bool isIgnoreCharacter(int);
static bool getline_list(line_t*, int32_t, FILE*);

line_t* create_file_buffer(const char* filename, line_end_e* newlineMode)
{
  FILE* filePtr;
  line_t* currentLine;
  line_t* previousLine = NULL;

  filePtr = fopen(filename, "r");

  if (filePtr == NULL) {
    return NULL;
  }

  *newlineMode = getFileLineEnding(filePtr);

  do {
    currentLine = create_empty_line();
    currentLine->previous = previousLine;
    if (previousLine != NULL) {
      previousLine->next = currentLine;
    }
    previousLine = currentLine;
  } while (!getline_list(previousLine, LINE_MAX, filePtr));

  // Seek back to start
  while (currentLine->previous != NULL) {
    currentLine = currentLine->previous;
  }

  return currentLine; // return first line
}

static line_end_e getFileLineEnding(FILE* stream)
{
  int c;

  do {
    c = fgetc(stream);
    if (c == (int)'\r') {
      c = fgetc(stream);
      if(c == (int)'\n') {
        rewind(stream);
        return DOS_LINE_END;
      } else {
        rewind(stream);
        return OSX_LINE_END;
      }
    }
    if (c == (int)'\n') {
      rewind(stream);
      return UNIX_LINE_END;
    }
  } while (c != EOF);
  rewind(stream);
  return UNIX_LINE_END; // error unknown!!!
}

static bool getline_list(line_t* emptyLine,int32_t n, FILE* stream)
{
  int c;
  node_t* node;

  do {
    c = fgetc(stream);
    n--;

    if (!isIgnoreCharacter(c)) {
      emptyLine->base = create_empty_node();
      emptyLine->base->c = (char)c;
    }

  } while ((c != (int)'\n') && (c != EOF) && (n != 0) && 
           (emptyLine->base == NULL));

  node = emptyLine->base;

  while ((c != (int)'\n') && (c != EOF) && (n != 0)) {
    c = fgetc(stream);
    n--;

    if (!isIgnoreCharacter(c)) {
      node->next = create_empty_node();
      node = node->next;
      node->c = (char)c;
    }

  }

  return (c == EOF);
}

static bool isIgnoreCharacter(int c)
{
  return (c == '\r') || (c == '\n') || (c == EOF);
}
