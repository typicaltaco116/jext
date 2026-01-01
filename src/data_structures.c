#include "data_structures.h"

#include "types.h"

#include <stdlib.h>

node_t* create_empty_node(void)
{
  node_t* newNode;
  newNode = malloc(sizeof(node_t));
  newNode->next = NULL;
  return newNode;
}

line_t* create_empty_line(void)
{
  line_t* newLine;
  newLine = malloc(sizeof(line_t));
  newLine->next = NULL;
  newLine->previous = NULL;
  newLine->base = create_empty_node();
  return newLine;
}

void free_entire_list(node_t** base)
/*
  Frees the memory of all items in a linked list.
  Additionally, modifies the base pointer to null to prevent dangling.
*/
{
  node_t* temp;
  node_t* target;

  temp = *base;
  *base = NULL; // prevent dangling pointer

  while (temp != NULL) {
    target = temp;
    temp = temp->next;
    free(target);
  }
}

void free_line(line_t** line)
/*
  Frees the memory used by a line's struct and its linked list.
  Also handles the reattachment of the line's surroundings
*/
{
  line_t* previous;
  line_t* next;

  free_entire_list(&(*line)->base);

  previous = (*line)->previous;
  next = (*line)->next;
  if (previous != NULL) {
    previous->next = next;
  }
  if (next != NULL) {
    next->previous = previous;
  }

  free(*line);
  *line = NULL;
}

void free_all_lines(line_t** firstLine)
{
  line_t* lineToDelete;
  line_t* line;

  line = *firstLine;

  while (line != NULL) {
    lineToDelete = line;
    line = line->next;
    free_line(&lineToDelete);
  }

  *firstLine = NULL;
}

line_t* index_buffer(line_t* buffer, int32_t index)
{
  line_t* line = buffer;

  while ((index != 0) && (line != NULL)) {
    line = line->next;
    index--;
  }

  return line;
}
