CC=gcc
CFLAGS=-I.

DEPS=20212277.h
OBJ=20212277.o
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
	
20212277: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
