
all: test

test: source/test.c include/cflags.h
	$(CC) -o test -I include source/test.c
