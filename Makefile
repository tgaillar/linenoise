CC = gcc

COFLAGS = -Os
CDFLAGS = -g
CWFLAGS = -Wall -W -Wno-unused-parameter

all: linenoise_example

linenoise_example: linenoise.h linenoise.c Makefile

linenoise_example: linenoise.c example.c
	$(CC) $(CWFLAGS) $(COFLAGS) $(CDFLAGS) -o linenoise_example linenoise.c example.c

clean:
	rm -f linenoise_example
