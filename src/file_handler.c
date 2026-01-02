#include "file_handler.h"

#include "types.h"
#include "data_structures.h"

#include <stdio.h>

#define LINE_MAX 150

static line_end_e getFileLineEnding(FILE* stream);
static bool isIgnoreCharacter(int c);
static bool getline_list(node_t* baseNode,int32_t n, FILE* stream);

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
  } while (!getline_list(previousLine->base, LINE_MAX, filePtr));

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

static bool isIgnoreCharacter(int c)
{
  return (c == '\r') || (c == '\n') || (c == EOF);
}

static bool getline_list(node_t* baseNode,int32_t n, FILE* stream)
{
  int c;
  node_t* node;

  node = baseNode;

  // Priming read
  c = fgetc(stream);
  if (!isIgnoreCharacter(c)) {
    node->c = (char)c;
  }
  n--;

  while ((c != (int)'\n') && (c != EOF) && (n != 0)) {
    c = fgetc(stream);
    if (!isIgnoreCharacter(c)) {
      node->next = create_empty_node();
      node = node->next;
      node->c = (char)c;
    }
    n--;
  }
  // THIS IS SHIT

  node->next = NULL; // terminate list

  return (c == EOF);
}
