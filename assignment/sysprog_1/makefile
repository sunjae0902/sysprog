CC=gcc
CFLAGS=-I.

DEPS=functions.h
OBJ=functions.o
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
	
functions: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
