CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -g

SRC = src/polynomial.c src/polynomial.h
OBJ = obj/polynomial.o
LIB = lib/libpolynomial.a

PREFIX ?= /usr/local

.PHONY: all install uninstall clean

all: $(LIB)

install: $(LIB)
	mkdir -p $(PREFIX)/lib
	mkdir -p $(PREFIX)/include
	cp lib/libpolynomial.a $(PREFIX)/lib/libpolynomial.a
	cp src/polynomial.h $(PREFIX)/include/polynomial.h

$(LIB): $(OBJ)
	mkdir lib
	ar rcs $@ $^

$(OBJ): $(SRC)
	mkdir obj
	$(CC) $(CFLAGS) -c $< -o $@

uninstall:
	rm $(PREFIX)/lib/libpolynomial.a $(PREFIX)/include/polynomial.h

clean:
	git clean -Xdf

