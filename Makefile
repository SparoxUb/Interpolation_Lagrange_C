CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -g

SRC = src/polynomial.c src/polynomial.h
OBJ = obj/polynomial.o
LIB = lib/libpolynomial.a

INSTALL_LIB_DIR = /usr/local/lib
INSTALL_INCLUDE_DIR = /usr/local/include

.PHONY: all install clean

all: $(LIB)

install: $(LIB)
	sudo cp lib/libpolynomial.a	$(INSTALL_LIB_DIR)/libpolynomial.a
	sudo cp src/polynomial.h 	$(INSTALL_INCLUDE_DIR)/polynomial.h

$(LIB): $(OBJ)
	mkdir lib
	ar rcs $@ $^

$(OBJ): $(SRC)
	mkdir obj
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf lib/ obj/ *.a *.o *~

