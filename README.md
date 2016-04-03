# c-polynomial

c-polynomial is a simple implementation in C of a polynomial ADT that supports addition, subtraction, multiplication, differentation, integration, and other useful operations.

## Installation

To install the library locally, run:

    $ make install

## Usage

Include `polynomial.h` in your program.

```c
#include <stdio.h>
#include <polynomial.h>

int main(int argc, char **argv) {
    Polynomial *p = polynomial_new(5);

    polynomial_set_coefficient(p, 0, 7);
    polynomial_set_coefficient(p, 1, 2);
    polynomial_set_coefficient(p, 2, 3.5);
    polynomial_set_coefficient(p, 3, 0.9);
    polynomial_set_coefficient(p, 4, 18);
    polynomial_set_coefficient(p, 5, 1);
    
    printf("%lf", polynomial_get_coefficient(p, 2));

    polynomial_destroy(&p);
    
    return 0;
}
```

Statically link the program against the library and compile it.

```bash
$ gcc test.c -I/usr/local/include -static -L/usr/local/lib/ -lpolynomial -o test
```
