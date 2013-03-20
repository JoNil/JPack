CC = gcc
CFLAGS += -g -std=c99 -Wall -Wextra -Werror -Wconversion -pedantic -pedantic-errors
LDFLAGS +=

all: lib/libjpack.so

src/jpack.o: src/jpack.c
	$(CC) $(CFLAGS) -fPIC -c -I./include -o src/jpack.o src/jpack.c

lib/libjpack.so: src/jpack.o
	$(CC) $(LDFLAGS) -shared -o lib/libjpack.so src/jpack.o

test: jpack_test.bin

src/jpack_test.o: src/jpack.c
	$(CC) $(CFLAGS) -DTEST -c -I./include -o src/jpack_test.o src/jpack.c

jpack_test.bin: src/jpack_test.o
	$(CC) $(LDFLAGS) -o jpack_test.bin src/jpack_test.o

clean:
	rm -f src/jpack.o
	rm -f src/jpack_test.o
	rm -f lib/libjpack.so
	rm -f jpack_test.bin