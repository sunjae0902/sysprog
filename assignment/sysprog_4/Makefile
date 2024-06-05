CC = gcc
CFLAGS = -Wall -pthread

all: 20212277-mut 20212277-sem 20212277-cv

20212277-mut: 20212277-mut.c
	$(CC) $(CFLAGS) 20212277-mut.c -o 20212277-mut

20212277-sem: 20212277-sem.c
	$(CC) $(CFLAGS) 20212277-sem.c -o 20212277-sem

20212277-cv: 20212277-cv.c
	$(CC) $(CFLAGS) 20212277-cv.c -o 20212277-cv

clean:
	rm -f 20212277-mut 20212277-sem 20212277-cv

