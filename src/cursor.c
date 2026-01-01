#include "cursor.h"

#include "types.h"
#include "data_structures.h"

#include <stdlib.h>

#define DEFAULT_MAX_ROW     40 - 1
#define DEFAULT_MAX_COLUMN  80 - 1

static line_t* _buffer;
static line_t* _currentLine;
static node_t* _currentNode;

static int32_t _cursorRow;
static int32_t _cursorColumn;

void cursor_attach_buffer(line_t* buffer)
{
  _buffer = buffer;
}

void move_cursor(int32_t row, int32_t column)
{
  _cursorRow = _cursorColumn = 0;

  _currentLine = _buffer;
  while ((_cursorRow < row) && (_currentLine->next != NULL)) {
    _currentLine = _currentLine->next;
    _cursorRow++;
  }

  _currentNode = _currentLine->base;
  while ((_cursorColumn < column) && (_currentNode->next != NULL)) {
    _currentNode = _currentNode->next;
    _cursorColumn++;
  }
}

void get_cursor_pos(int32_t* row, int32_t* column)
{
  *row = _cursorRow;
  *column = _cursorColumn;
}

void walk_cursor(int32_t deltaRow, int32_t deltaColumn)
{
  if (deltaRow > 0) { // move down
    while ((deltaRow > 0) && (_currentLine->next != NULL)) {
      _currentLine = _currentLine->next;
      deltaRow--;
      _cursorRow++;
    }
  } else if (deltaRow < 0) { // move up
    while ((deltaRow < 0) && (_currentLine->previous != NULL)) {
      _currentLine = _currentLine->previous;
      deltaRow++;
      _cursorRow--;
    }
  }

  if (deltaColumn > 0) { // move right
    while ((deltaColumn > 0) && (_currentNode->next != NULL)) {
      _currentNode = _currentNode->next;
      deltaColumn--;
      _cursorColumn++;
    }
  } else if (deltaColumn < 0) { // move left
    _currentNode = _currentLine->base;
    _cursorColumn += deltaColumn;
    if (_cursorColumn < 0) { // leftmost collision check
      _cursorColumn = 0;
    }
    for (int i = _cursorColumn; i != 0; i--) { // no pointer checks necessary
      _currentNode = _currentNode->next;       // all nodes before must exist
    }
  }
}

void insert_value_after_cursor(char c)
{
  node_t* newNode;
  node_t* oldNextNode;

  newNode = create_empty_node();
  newNode->c = c;
  oldNextNode = _currentNode->next;

  // Reattach
  _currentNode->next = newNode;
  newNode->next = oldNextNode;

  // Increment cursor
  _currentNode = _currentNode->next;
  _cursorColumn++;
}

static node_t* getPreviousNode(line_t* line, int32_t column)
{
  node_t* node;
  node = line->base;
  
  if (column <= 0) {
    return NULL;
  }

  column -= 1;

  while (column != 0) {
    node = node->next;
    column--;
  }
  return node;
}

void delete_value_under_cursor(void)
{
  node_t* previousNode;
  node_t* nextNode;

  previousNode = getPreviousNode(_currentLine, _cursorColumn);
  nextNode = _currentNode->next;

  // We need to leave at least one node in each line
  if ((nextNode == NULL) && (previousNode == NULL)) { // only character in the row
    _currentNode->c = '\0';
    return;
  }

  // Reattach previous node if it exists
  if (previousNode == NULL) {
    _currentLine->base = nextNode;
  } else {
    previousNode->next = nextNode;
  }

  if (nextNode == NULL) { // If and only if we are at the end of the line
    free(_currentNode);   // does the cursor change position
    _currentNode = previousNode;
    _cursorColumn--;      
  } else {
    free(_currentNode);
    _currentNode = nextNode;
  }
}
