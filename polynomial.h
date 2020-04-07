#ifndef __POLYNOMIAL_H__
#define __POLYNOMIAL_H__

#include <stdlib.h>
#include <stdbool.h>
#include "Fraction.h"

typedef struct polynomial_s Polynomial;
typedef struct Fraction_s Fraction;

Polynomial  *polynomial_new                     (size_t degree);
Polynomial  *polynomial_new_tab                     (Fraction **tab,size_t degree);

Polynomial  *polynomial_copy                    (Polynomial *p);
void         polynomial_destroy                 (Polynomial **p);

size_t       polynomial_get_degree              (Polynomial *p);
void polynomial_set_coefficient(Polynomial *p, size_t i, Fraction *a);
Fraction * polynomial_get_coefficient(Polynomial *p, size_t i);

bool         polynomial_is_zero                 (Polynomial *p);
bool         polynomial_equals                  (Polynomial *p1, Polynomial *p2);


Polynomial  *polynomial_add                     (Polynomial *p1, Polynomial *p2);
Polynomial  *polynomial_subtract                (Polynomial *p1, Polynomial *p2);
Polynomial  *polynomial_multiply                (Polynomial *p1, Polynomial *p2);
Polynomial *polynomial_multiply_by_constant(Polynomial *p, Fraction *c);



Polynomial  *polynomial_symmetric               (Polynomial *p);
char * Poly_ToString(Polynomial *P);



#endif /* __POLYNOMIAL_H__ */
