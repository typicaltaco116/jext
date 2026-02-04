sources = src/main.c src/terminal.c src/drawing.c src/input.c \
	  src/data_structures.c src/file_handler.c src/cursor.c src/toolbar.c
objects = build/main.o build/terminal.o build/drawing.o build/input.o \
	  build/data_structures.o build/file_handler.o build/cursor.o \
	  build/toolbar.o

linking_flags = -lncurses -ltinfo

CC = gcc
CFLAGS = -Wall -std=c99

#Default Target
jext: CFLAGS += -O3
jext: $(objects)
	$(CC) -o jext $^ $(linking_flags) $(CFLAGS)

static-build: CFLAGS += -static
static-build: jext
debug: CFLAGS += -g
debug: jext
	gdb jext


build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build/main.o: $(filter-out build/main.o, $(objects)) 

$(objects): src/types.h

build/terminal.o: src/terminal.h src/ANSI_codes.h

build/input.o: src/input.h src/ANSI_codes.h src/cursor.h src/drawing.h \
	src/terminal.h src/toolbar.h

build/drawing.o: src/drawing.h src/ANSI_codes.h src/data_structures.h \
	src/cursor.h src/terminal.h

build/data_structures.o: src/data_structures.h

build/file_handler.o: src/file_handler.h src/data_structures.h

build/cursor.o: src/cursor.h src/data_structures.h

build/toolbar.o: src/toolbar.h src/terminal.h

build:
	mkdir -p build/

.PHONY: clean
clean:
	rm $(objects)
