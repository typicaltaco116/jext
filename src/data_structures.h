#pragma once

typedef struct node_s {
  struct node_s* next;
  char c; 
} node_t;

typedef struct line_s {
  struct line_s* next;
  struct line_s* previous;
  node_t* base; // ptr to linked list head for the line
} line_t;

node_t* create_empty_node(void);

line_t* create_empty_line(void);

void free_entire_list(node_t** base);

void free_line(line_t** line);

void free_all_lines(line_t** firstLine);
