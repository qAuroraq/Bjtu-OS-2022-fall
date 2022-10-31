SOURCE = $(wildcrad *.c)
TARGETS = $(patsubst %.c, %, $(SOURCE))
CC = gcc
CFLAGS = -Wall -g
all:$(TARGETS)
$(TARGETS):%:%.C
	$(CC) $< $(CFLAGS) -o $@
.PHONY:clean all
clean:
	rm *.o
