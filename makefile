CC       = gcc
CPPFLAGS =
CFLAGS   = -Wall -Wextra -Wno-implicit-fallthrough -O2 -std=c17
LDFLAGS  =

.PHONY: all clean

all: labyrinth

labyrinth: main.o
	$(CC) -o labyrinth $^
clean:
	rm -f *.o labyrinth
