SOURCES = src/main.c src/terminal.c src/drawing.c src/input.c \
	  src/data_structures.c src/file_handler.c src/cursor.c src/toolbar.c
OBJECTS = $(SOURCES:src/%.c=build/%.o)

LD_FLAGS = $(shell pkg-config --libs ncurses)

CC = gcc
CFLAGS = -Wall -std=c99
OP_FLAGS = -O3

# Default Target
all: CLAGS += $(OP_FLAGS)
all: jext

jext: $(OBJECTS)
	$(CC) -o jext $^ $(LD_FLAGS) $(CFLAGS)

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build:
	mkdir -p build/

static-build: CFLAGS += -static
static-build: jext

debug: CFLAGS += -g
debug: all

.PHONY: clean
clean:
	rm $(OBJECTS)
	rmdir build/
