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
  while ((_cursorColumn < column) && (_currentNode != NULL)) {
    _currentNode = _currentNode->next;
    _cursorColumn++;
  }
}

void get_cursor_pos(int32_t* row, int32_t* column)
{
  *row = _cursorRow;
  *column = _cursorColumn;
}

line_t* get_cursor_line(void)
{
  return _currentLine;
}

node_t* get_cursor_node(void)
{
  return _currentNode;
}

static void walkCursorRow(int32_t deltaRow)
// Resets the current node and _cursorColumn
{
  if (deltaRow == 0) {
    return;
  }

  if (deltaRow > 0) { // move down
    while ((deltaRow > 0) && (_currentLine->next != NULL)) {
      _currentLine = _currentLine->next;
      deltaRow--;
      _cursorRow++;
    }
  } else { // move up
    while ((deltaRow < 0) && (_currentLine->previous != NULL)) {
      _currentLine = _currentLine->previous;
      deltaRow++;
      _cursorRow--;
    }
  }

  _currentNode = _currentLine->base;
  _cursorColumn = 0;
}

static void walkCursorColumn(int32_t deltaColumn)
{
  if (deltaColumn == 0) {
    return;
  }

  if (deltaColumn > 0) { // move right
    while ((deltaColumn > 0) && (_currentNode != NULL)) {
      _currentNode = _currentNode->next;
      deltaColumn--;
      _cursorColumn++;
    }
  } else { // move left
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

void walk_cursor(int32_t deltaRow, int32_t deltaColumn)
{

  if (deltaRow != 0) {
    int32_t originalColumn = _cursorColumn;
    walkCursorRow(deltaRow);
    walkCursorColumn(originalColumn); // attempt to return to the original column
  }
  if (deltaColumn != 0) {
    walkCursorColumn(deltaColumn);
  }
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

void insert_value_on_cursor(char c)
{
  node_t* prevNode;
  node_t* newNode;

  prevNode = getPreviousNode(_currentLine, _cursorColumn);

  newNode = create_empty_node();
  newNode->next = _currentNode;
  newNode->c = c;

  if (prevNode != NULL) {
    prevNode->next = newNode;
  } else {
    _currentLine->base = newNode; // Reattach the line base
  }

  _cursorColumn++;
}

void delete_value_before_cursor(void)
{
  node_t* previousNode;
  node_t* prevPrevNode;

  previousNode = getPreviousNode(_currentLine, _cursorColumn);
  prevPrevNode = getPreviousNode(_currentLine, _cursorColumn - 1);

  if (previousNode == NULL) { // must be at the base of a line
    return;
  }

  _cursorColumn--;

  if (prevPrevNode == NULL) {
    free(previousNode);
    _currentLine->base = _currentNode; // !!!!! POTENTIAL PROBLEMS
    return;
  }

  free(previousNode);
  prevPrevNode->next = _currentNode;
}
