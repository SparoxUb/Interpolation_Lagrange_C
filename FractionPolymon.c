#include "FractionPolymon.h"
#include "polymonial.c"



 ///// polimonial fration =  polinom1 / polinom2
struct PolFra_s
{
    Polynomial p1;
    Polynomial p2;
};



PolFra * PolFra_new( Polynomial *p1, Polynomial *p2 ){

    PolFra * MyFraPol = ( PolFra * ) malloc( sizeof(PolFra) );

    MyFraPol->p1 = polynomial_copy(p1);
    MyFraPol->p2 = polynomial_copy(p2);

    if( !MyFraPol->p1 || MyFraPol->p2 )
        return NULL;

    return MyFraPol;
}


