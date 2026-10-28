CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror -Isrc -Ivendor/unity

.PHONY: test clean

test: build/test_door
	./build/test_door

build/test_door: test/test_door.c src/door.c src/door.h vendor/unity/unity.c
	mkdir -p build
	$(CC) $(CFLAGS) -o $@ test/test_door.c src/door.c vendor/unity/unity.c

clean:
	rm -rf build
