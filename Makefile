CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -g

all: libpolynomial.a

libpolynomial.a: polynomial.o
	ar rcs $@ $^

polynomial.o: polynomial.c polynomial.h
	$(CC) $(CFLAGS) -c $< -o $@

install: libpolynomial.a
	sudo mv libpolynomial.a	/usr/local/lib/libpolynomial.a
	sudo cp polynomial.h /usr/local/include/polynomial.h

clean:
	rm -f *.a *.o *~

