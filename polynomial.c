#include "polynomial.h"
#include "Fraction.c"

struct polynomial_s
{
    size_t degree;
    Fraction **terms;
};


static bool          polynomial_valid_index  (Polynomial *p, size_t i);
static Polynomial   *polynomial_bigger      (Polynomial *p1, Polynomial *p2);
static Polynomial   *polynomial_smaller     (Polynomial *p1, Polynomial *p2);
static Polynomial   *polynomial_zero        (void);
Polynomial   *polynomial_reduce      (Polynomial *p);

/**
 * Allocates memory for a new polynomial of degree n.
 *
 * @param[in] n the degree of the polynomial
 *
 * @return a new polynomial or NULL if memory allocation fails
 */
Polynomial *polynomial_new(size_t n)
{
    Polynomial *p = malloc(sizeof(struct polynomial_s));
    if (!p)
        return NULL;

    p->terms = (Fraction **) calloc(n + 1, sizeof(Fraction*));

    for(int i=0; i<=n; i++)
    {

        p->terms[i] = NFRA_new_Fraction(1,1);

    }

    if (!p->terms)
    {
        free(p);
        return NULL;
    }

    p->degree = n;

    return p;
}



Polynomial  *polynomial_new_tab (Fraction **tab,size_t n){

    Polynomial *p = malloc(sizeof(struct polynomial_s));
    if (!p)
        return NULL;

            p->terms = (Fraction **) calloc(n + 1, sizeof(Fraction*));

    for(int i=0; i<=n; i++)
    {
        if(tab[i])
            p->terms[i] = NFRA_Fraction_copy( tab[i] ) ;
        else
            p->terms[i] = NFRA_new_Fraction(0,1) ;
    }

    if (!p->terms)
    {
        free(p);
        return NULL;
    }

    p->degree = n;

    return p;

}




/**
 * Copies a polynomial to a new memory location.
 *
 * @param[in] p the polynomial to be copied
 *
 * @return the copy of the polynomial
 */
Polynomial *polynomial_copy(Polynomial *p)
{
    if (p == NULL)
        return NULL;

    Polynomial *copy = polynomial_new(p->degree);
    if (!copy)
        return NULL;

    for (size_t i = 0; i <= p->degree; i++)
        copy->terms[i] = NFRA_Fraction_copy(p->terms[i]);

    return copy;
}




/**
 * Frees up the allocated memory and the pointer of a polynomial.
 *
 * @param[in] p the polynomial to be destroyed
 */
void polynomial_destroy(Polynomial **p)
{
    if (*p == NULL)
        return;

    for (size_t i = 0; i <= (*p)->degree; i++)
        NFRA_Fraction_destroy(&(*p)->terms[i]);

    free((*p)->terms);
    free(*p);

    *p = NULL;
}

/**
 * Returns the degree of a polynomial.
 *
 * @param[in] p the polynomial
 *
 * @return the degree of the polynomial
 */
size_t polynomial_get_degree(Polynomial *p)
{
    if (p == NULL)
        return -1;

    return p->degree;
}

/**
 * Sets the coefficient of the term of exponent i to a.
 *
 * @param[in] p the polynomial
 * @param[in] i the exponent
 * @param[in] a the coefficient
 */
void polynomial_set_coefficient(Polynomial *p, size_t i, Fraction *a)
{
    if (p == NULL || !polynomial_valid_index(p, i))
        return;

    p->terms[i] = a;
/*
    if ( NFRA_check_null(a) )
        p = polynomial_reduce(p);*/
}

/**
 * Returns the coefficient of the term of exponent i.
 *
 * @param[in] p the polynomial
 * @param[in] i the exponent
 *
 * @return the coefficient of the term of exponent i
 */

Fraction * polynomial_get_coefficient(Polynomial *p, size_t i)
{
    if (p == NULL || !polynomial_valid_index(p, i))
        return NFRA_new_Fraction(0,1);

    return p->terms[i];
}

/**
 * Checks if a polynomial p is a zero polynomial, i.e. p(x) = 0.
 *
 * @param[in] p the polynomial
 *
 * @return true if the polynomial is a zero polynomial
 */
bool polynomial_is_zero(Polynomial *p)
{
    if (p == NULL)
        return false;

    for (size_t i = 0; i <= p->degree; i++)
        if (p->terms[i] != 0)
            return false;

    return true;
}

/**
 * Checks if a polynomial equals another.
 *
 * @param[in] p1 the first polynomial
 * @param[in] p2 the second polynomial
 *
 * @return true if the two polynomials are equal
 */
bool polynomial_equals(Polynomial *p1, Polynomial *p2)
{
    if (p1 == NULL || p2 == NULL)
        return false;
    if (p1->degree != p2->degree)
        return false;

    for (size_t i = 0; i <= p1->degree; i++)
        if (  ! NFRA_equal(p1->terms[i] , p2->terms[i]) )
            return false;

    return true;
}

/**
 * Evaluates a polynomial p at x, i.e. computes p(x), using Horner's method.
 *
 * @param[in] p the polynomial
 * @param[in] x
 *
 * @return the value of p at x
 */


/**
 * Adds two polynomials.
 *
 * @param[in] p1 the first polynomial
 * @param[in] p2 the second polynomial
 *
 * @return the result of adding the two polynomials
 */
Polynomial *polynomial_add(Polynomial *p1, Polynomial *p2)
{
    if (p1 == NULL || p2 == NULL)
        return NULL;

    Polynomial *smaller = polynomial_smaller(p1, p2);
    Polynomial *bigger = polynomial_bigger(p1, p2);

    size_t n = smaller->degree;
    size_t m = bigger->degree;

    Polynomial *result = polynomial_new(m);
    if (!result)
        return NULL;

    size_t i;

    if(smaller==p1){
    for (i = 0; i <= n; i++)
        result->terms[i+m-n] = NFRA_add( p1->terms[i], p2->terms[i+m-n] );

    if(m!=n)
    for (i = 0; i < m-n; i++)
        result->terms[i] = NFRA_Fraction_copy(bigger->terms[i]);

    }else{

        for (i = 0; i <= n; i++)
        result->terms[i+m-n] = NFRA_add(p1->terms[i+m-n], p2->terms[i]);

        if(m!=n)
        for (i = 0; i < m-n; i++)
            result->terms[i] = NFRA_Fraction_copy(bigger->terms[i]);
    }




    return polynomial_reduce(result);
}

/**
 * Subtracts two polynomials.
 *
 * @param[in] p1 the first polynomial
 * @param[in] p2 the second polynomial
 *
 * @return the result of subtracting the two polynomials
 */
Polynomial *polynomial_subtract(Polynomial *p1, Polynomial *p2)
{
    if (p1 == NULL || p2 == NULL)
        return NULL;

    Polynomial *smaller = polynomial_smaller(p1, p2);
    Polynomial *bigger = polynomial_bigger(p1, p2);


    size_t n = smaller->degree;
    size_t m = bigger->degree;

    Polynomial *result = polynomial_new(m);
    if (!result)
        return NULL;


    size_t i;


    if(smaller==p1){
    for (i = 0; i <= n; i++)
        result->terms[i+m-n] = NFRA_soustract(p1->terms[i], p2->terms[i+m-n]);

    if(m!=n)
    for (i = 0; i < m-n; i++)
        result->terms[i] = NFRA_multiplication(bigger->terms[i], NFRA_new_Fraction(-1,1)) ;

    }else{

        for (i = 0; i <= n; i++)
        result->terms[i+m-n] = NFRA_soustract(p1->terms[i+m-n], p2->terms[i]);

        if(m!=n)
        for (i = 0; i < m-n; i++)
            result->terms[i] = NFRA_multiplication(bigger->terms[i], NFRA_new_Fraction(1,1)) ;

    }

    return polynomial_reduce(result);

}

/**
 * Multiplies two polynomials.
 *
 * @param[in] p1 the first polynomial
 * @param[in] p2 the second polynomial
 *
 * @return the result of multiplying the two polynomials
 */
Polynomial * polynomial_multiply(Polynomial *p1, Polynomial *p2)
{
    if (p1 == NULL || p2 == NULL)
        return NULL;
    int num = (int) p1->degree + p2->degree;
    Polynomial *result = polynomial_new( num );
    if (!result)
        return NULL;

    for(;num>=0;num--)
        result->terms[num]= NFRA_new_Fraction(0,1);


    for (size_t i = 0; i <= p1->degree; i++)
        for (size_t j = 0; j <= p2->degree; j++)
            result->terms[i+j] = NFRA_add(result->terms[i+j], NFRA_multiplication(p1->terms[i], p2->terms[j]) );


    return polynomial_reduce(result);
}

/**
 * Multiplies a polynomial by a constant.
 *
 * @param[in] p the polynomial to be multiplied
 * @param[in] c the constant to multiply by
 *
 * @return the result of multiplying the polynomial by the constant
 */
Polynomial *polynomial_multiply_by_constant(Polynomial *p, Fraction *c)
{
    if (p == NULL)
        return NULL;
    if (c == 0)
        return polynomial_zero();

    Polynomial *result = polynomial_new(p->degree);
    if (!result)
        return NULL;

    for (size_t i = 0; i <= p->degree; i++)
        result->terms[i] = NFRA_multiplication( p->terms[i], c);

    return result;
}

/**
 * Calculates the symmetric of a polynomial.
 *
 * @param[in] p the polynomial
 *
 * @return the symmetric polynomial
 */
Polynomial *polynomial_symmetric(Polynomial *p)
{
    return polynomial_multiply_by_constant(p, NFRA_new_Fraction(-1,1) );
}




/**
 * Computes the definite integral of a polynomial in the interval [a, b].
 *
 * @param[in] p the polynomial to be integrated
 */

/**
 * Checks if an index is valid in the terms array.
 *
 * @param[in] p the polynomial
 * @param[in] i the index
 *
 * @return true if the index is valid in the terms array
 */
static bool polynomial_valid_index(Polynomial *p, size_t i)
{
    if (p == NULL)
        return false;
    if (i >= p->degree + 1)
        return false;

    return true;
}

/**
 * Returns the highest degree polynomial.
 *
 * @param[in] p1 the first polynomial
 * @param[in] p2 the second polynomial
 *
 * @return the highest degree polynomial
 */
static Polynomial *polynomial_bigger(Polynomial *p1, Polynomial *p2)
{
    if (p1 == NULL || p2 == NULL)
        return NULL;

    return (p1->degree > p2->degree) ? p1 : p2;
}

/**
 * Returns the lowest degree polynomial.
 *
 * @param[in] p1 the first polynomial
 * @param[in] p2 the second polynomial
 *
 * @return the lowest degree polynomial
 */
static Polynomial *polynomial_smaller(Polynomial *p1, Polynomial *p2)
{
    if (p1 == NULL || p2 == NULL)
        return NULL;

    return (p1->degree > p2->degree) ? p2 : p1;
}

/**
 * Returns the zero polynomial, i.e. p(x) = 0.
 *
 * @return the zero polynomial
 */
static Polynomial *polynomial_zero()
{
    return polynomial_new(0);
}

/**
 * Reduces a polynomial to its canonical form.
 *
 * @param[in] p the polynomial to be reduced
 */
Polynomial *polynomial_reduce(Polynomial *p)
{
    if (p == NULL)
        return NULL;

    size_t degree = p->degree;

    //printf(" \n\n Degree => %d   Fractions  => ",degree);

        //for(int i=0;i<=p->degree;i++)
        //printf("\t here %d : %s",i,NFRA_ToString( polynomial_get_coefficient( p, i ) ) ) ;

    for(int i=0;i<=p->degree;i++){


        if( NFRA_check_null( polynomial_get_coefficient( p, i )) && degree>0 ){
           degree--;
           for(int j=i;j<p->degree;j++)
                if( polynomial_get_coefficient( p, j+1 )  )
                    polynomial_set_coefficient(p,j,  polynomial_get_coefficient( p, j+1 ) );
           }else
            break;
    }

       // printf(" \n\n term[%d] = %s ",degree,NFRA_ToString( polynomial_get_coefficient( p,0 ) ));

    if (degree != p->degree)
        p->terms = realloc(p->terms, (degree+1) * sizeof(Fraction));

    p->degree = degree;
    return p;
}

int check_All_null(Polynomial *P){
    for(int i=0;i<=P->degree;i++){
        if( !NFRA_check_null(P->terms[i]) )
            return 1;
    }
    return 0;
}

char * Poly_ToString(Polynomial *P)
{
    char tmp[100];
    char toReturn[1024]=" ";

    if( !check_All_null(P)){
     sprintf(toReturn," 0 ");
     }
    else
    for(int i=0 ; i<=P->degree; i++)
    {
        if( !NFRA_check_null( polynomial_get_coefficient(P,i) ) )
        {

            /// well writing the signal of the next number
            if( NFRA_check_posi( P->terms[i] ) ){
                if( i )
                    sprintf(tmp," + ");
                }
            else
                sprintf(tmp," - ");


            if( NFRA_checkSingle(P->terms[i]) ){

                if( P->degree-i==0 )
                    sprintf( tmp, " %s 1 ", trim(tmp));
                else if(P->degree-i==1)
                    sprintf( tmp, " %s x ", trim(tmp));
                else
                    sprintf( tmp, " %s (x^%d) ", trim(tmp), P->degree-i);            /// not writing coefficients

            }else{

                if( P->degree-i==0 )
                    sprintf( tmp, " %s %s  ",trim(tmp), NFRA_ToString(P->terms[i]));
                else if(P->degree-i==1)
                    sprintf( tmp, " %s %s * x ",trim(tmp), NFRA_ToString(P->terms[i]));
                else
                sprintf( tmp, " %s %s * (x^%d) ",trim(tmp), NFRA_ToString(P->terms[i]),P->degree-i);

                }
        strcat(toReturn,tmp);
        }

    }
    return trim(toReturn);
}






