#ifndef FRACTIONPOLYMON_H_INCLUDED
#define FRACTIONPOLYMON_H_INCLUDED


#include "polynomial.h"


typedef struct PolFra_s PolFra;
typedef struct polynomial_s Polynomial;
typedef struct Fraction_s Fraction;


PolFra * PolFra_new( Polynomial *p1, Polynomial *p2 );
PolFra * PolFra_addition( PolFra *p1, PolFra *p2 );
PolFra * PolFra_multiply( PolFra *p1, PolFra *p2 );
PolFra * PolFra_multiply_constant( PolFra *P, Fraction *F );


#endif // FRACTIONPOLYMON_H_INCLUDED
