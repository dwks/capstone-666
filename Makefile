CC = gcc
CFLAGS = -std=gnu99 -g -I /usr/include/capstone
LDFLAGS = -lcapstone

.PHONY: all
all: test

test: test.o example.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

.PHONY: clean
clean:
	-rm test test.o example.o
