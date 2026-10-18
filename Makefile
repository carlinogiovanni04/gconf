CC = clang
CFLAGS = -O0 -g -Wall -Werror -Wextra -D_CRT_SECURE_NO_WARNINGS -Iinclude/
SRC = examples/example.c
OUT = bin/example.exe

phony: clean all

all: $(OUT)
$(OUT): $(SRC)
	@mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(OUT)
