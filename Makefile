CC = gcc

CFLAGS = 
DFLAGS = -g -O0
WFLAGS = -Wall -Wno-unused-parameter -Wno-unused-variable -Wno-unused-but-set-variable -Wno-write-strings
ifneq (,$(strip $(DEBUG)))
CFLAGS += -DDEBUG=1
endif

CFLAGS += $(DFLAGS) $(WFLAGS)

all:  linenoise_example linenoise_utf8_example linenoise_cpp_example

linenoise_example: linenoise.h linenoise.c example.c
	$(CC) $(CFLAGS) -o $@ linenoise.c example.c

linenoise_utf8_example: linenoise.c utf8.c example.c
	$(CC) -DNO_COMPLETION -DUSE_UTF8 $(CFLAGS) -g -o $@ linenoise.c utf8.c example.c

linenoise_cpp_example: linenoise.h linenoise.c
	$(patsubst %gcc,%g++,$(CC)) $(CFLAGS) -g -o $@ linenoise.c example.c

clean:
	rm -f linenoise_example linenoise_utf8_example linenoise_cpp_example *.o

windows:
	$(MAKE) CC=i686-w64-mingw32-gcc
