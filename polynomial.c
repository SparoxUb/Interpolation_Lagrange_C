#include "polynomial.h"

struct polynomial_s
{
    size_t degree;
    double *terms;
};

static bool         polynomial_is_null      (Polynomial p);
static bool         polynomial_valid_index  (Polynomial p, size_t i);
static Polynomial   polynomial_bigger       (Polynomial p1, Polynomial p2);
static Polynomial   polynomial_smaller      (Polynomial p1, Polynomial p2);
static Polynomial   polynomial_zero         (void);
static Polynomial   polynomial_reduce       (Polynomial p);

/**
 * Allocates memory for a new polynomial of degree n.
 *
 * @param[in] n the degree of the polynomial
 *
 * @return a new polynomial or NULL if memory allocation fails
 */
Polynomial polynomial_new(size_t n)
{
    if(n < 0) {
        return NULL;
    }

    Polynomial p;

    if((p = malloc(sizeof(struct polynomial_s))) == NULL) {
        return NULL;
    }

    if((p->terms = calloc(n + 1, sizeof(double))) == NULL) {
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
Polynomial polynomial_copy(Polynomial p)
{
    if(polynomial_is_null(p)) return NULL;

    Polynomial copy;

    if((copy = polynomial_new(p->degree)) == NULL) return NULL;

    for(size_t i = 0; i < p->degree + 1; i++) {
        copy->terms[i] = p->terms[i];
    }

    return copy;
}

/**
 * Frees up the allocated memory of a polynomial.
 *
 * @param[in] p the polynomial to be destroyed
 */
void polynomial_destroy(Polynomial *p)
{
    if(polynomial_is_null(p)) return;

    free((*p)->terms);
    free(p);

    *p = NULL; // TODO: necessary?
}

/**
 * Returns the degree of a polynomial.
 *
 * @param[in] p the polynomial
 *
 * @return the degree of the polynomial
 */
size_t polynomial_get_degree(Polynomial p)
{
    if(polynomial_is_null(p)) return -1;

    return p->degree;
}

/**
 * Sets the coefficient of the term with exponent i to a.
 *
 * @param[in] p the polynomial
 * @param[in] p the exponent
 * @param[in] a the coefficient
 */
void polynomial_set_coefficient(Polynomial p, size_t i, double a)
{
    if(polynomial_is_null(p) || !polynomial_valid_index(p, i)) return;

    p->terms[i] = a;

    if(a == 0.0) p = polynomial_reduce(p);
}

/**
 * Returns the coefficient of the term with exponent i.
 *
 * @param[in] p the polynomial
 * @param[in] i the exponent
 *
 * @return the coefficient of the term with exponent i
 */
double polynomial_get_coefficient(Polynomial p, size_t i)
{
    if(polynomial_is_null(p) || !polynomial_valid_index(p, i)) return 0.0;

    return p->terms[i];
}

/**
 * Checks if a polynomial p is a zero polynomial, i.e. p(x) = 0.
 *
 * @param[in] p the polynomial
 *
 * @return true if the polynomial is a zero polynomial
 */
bool polynomial_is_zero(Polynomial p)
{
    if(polynomial_is_null(p)) return false;

    for(int i = 0; i < p->degree + 1; i++) {
        if(p->terms[i] != 0) return false;
    }

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
bool polynomial_equals(Polynomial p1, Polynomial p2)
{
    if(polynomial_is_null(p1) || polynomial_is_null(p2)) return false;

    if(p1->degree != p2->degree) return false;

    for(int i = 0; i < p1->degree + 1; i++) {
        if(p1->terms[i] != p2->terms[i]) return false;
    }

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
double polynomial_evaluate(Polynomial p, double x)
{
    if(polynomial_is_null(p)) return 0.0;

    double result = 0;

    for(int i = 0; i < p->degree + 1; i++) {
        result = result * x + p->terms[i];
    }

    return result;
}

/**
 * Adds two polynomials.
 *
 * @param[in] p1 the first polynomial
 * @param[in] p2 the second polynomial
 *
 * @return the result of adding the two polynomials
 */
Polynomial polynomial_add(Polynomial p1, Polynomial p2)
{
    if(polynomial_is_null(p1) || polynomial_is_null(p2)) return NULL;

    Polynomial result = polynomial_bigger(p1, p2);

    for(int i = 0; i < polynomial_smaller(p1, p2)->degree + 1; i++) {
        result->terms[i] += p1->terms[i] + p2->terms[i];
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
Polynomial polynomial_subtract(Polynomial p1, Polynomial p2)
{
    if(polynomial_is_null(p1) || polynomial_is_null(p2)) return NULL;

    return polynomial_reduce(polynomial_add(p1, polynomial_symmetric(p2)));
}

/**
 * Multiplies two polynomials.
 *
 * @param[in] p1 the first polynomial
 * @param[in] p2 the second polynomial
 *
 * @return the result of multiplying the two polynomials
 */
Polynomial polynomial_multiply(Polynomial p1, Polynomial p2)
{
    if(polynomial_is_null(p1) || polynomial_is_null(p2)) return NULL;

    Polynomial result;

    if((result = polynomial_new(p1->degree + p2->degree)) == NULL) {
        return NULL;
    }

    for(int i = 0; i < p1->degree + 1; i++) {
        for(int j = 0; i < p2->degree + 1; j++) {
            result->terms[i+j] += p1->terms[i] * p2->terms[j];
        }
    }

    return polynomial_reduce(result);
}

/**
 * Calculates the symmetric of a polynomial.
 *
 * @param[in] p the polynomial
 *
 * @return the symmetric polynomial
 */
Polynomial polynomial_symmetric(Polynomial p)
{
    if(polynomial_is_null(p)) return NULL;

    for(int i = 0; i < p->degree + 1; i++) {
        p->terms[i] *= -1;
    }

    return p;
}

/**
 * Computes the derivative of a polynomial.
 *
 * @param[in] p the polynomial
 * @param
 */
Polynomial polynomial_derivative(Polynomial p)
{
    if(polynomial_is_null(p)) return NULL;

    if(polynomial_get_degree(p) < 1) return polynomial_zero();

    Polynomial result = polynomial_new(p->degree - 1);

    result->terms[0] = 0.0;

    for(size_t i = 1; i < p->degree + 1; i++) {
        result->terms[i-1] = p->terms[i] * i;
    }

    return result;
}

/**
 * Computes the indefinite integral of a polynomial.
 *
 * @param[in] p the polynomial to be integrated
 *
 * @return the integrated polynomial
 */
Polynomial polynomial_indefinite_integral(Polynomial p, double c)
{
    if(polynomial_is_null(p)) return NULL;

    Polynomial result = polynomial_new(p->degree + 1);

    result->terms[0] = c; // TODO: worth it?

    for(size_t i = 0; i < p->degree + 1; i++) {
        result->terms[i+1] = p->terms[i] / (i+1);
    }

    return result;
}

/**
 * Computes the definite integral of a polynomial in the interval [a, b].
 *
 * @param[in] p the polynomial to be integrated
 */
double polynomial_definite_integral(Polynomial p, double a, double b)
{
    if(polynomial_is_null(p)) return 0.0;

    return polynomial_evaluate_at(polynomial_indefinite_integral(p, 0), b) - polynomial_evaluate_at(polynomial_indefinite_integral(p, 0), a);
}

/**
 * Writes a polynomial to a file.
 *
 * @param[in] p the polynomial
 * @param[in] filename the filename of the file to write to
 */
void polynomial_to_file(Polynomial p, char *filename)
{
    if(polynomial_is_null(p)) return;

    FILE *file;

    if((file = fopen(filename, "w")) == NULL) {
        return;
    }

    fprintf(file, "%zu\n", p->degree + 1);

    for(int i = 0; i < p->degree + 1; i++) {
        fprintf(file, "%lf\n", p->terms[i]);
    }

    fclose(file);
}

/**
 * Reads a polynomial from a file.
 *
 * @param[in] filename the filename of the file to read from
 *
 * @return the polynomial
 */
Polynomial polynomial_from_file(char *filename)
{
    Polynomial p;
    FILE *file;
    size_t degree;

    if((file = fopen(filename, "r")) == NULL) {
        return NULL;
    }

    fscanf(file, "%zu", &degree);

    if(degree < 0) {
        fclose(file);
        return NULL;
    }

    if((p = polynomial_new(degree)) == NULL) {
        fclose(file);
        return NULL;
    }

    for(int i = 0; i < degree; i++) {
        fscanf(file, "%lf", p->terms+i); // TODO
    }

    return NULL;
}



/**
 * Checks whether a polynomial is null.
 *
 * @param[in] p the polynomial to be checked
 *
 * @return true if the polynomial is null
 */
static bool polynomial_is_null(Polynomial p)
{
    if(p == NULL) {
        return false;
    }

    return true;
}

/**
 * Checks if an index is valid in the terms array.
 *
 * @param[in] p the polynomial
 * @param[in] i the index
 *
 * @return true if the index is valid in the terms array
 */
static bool polynomial_valid_index(Polynomial p, size_t i)
{
    if(polynomial_is_null(p)) return false;

    if(i >= p->degree + 1) {
        return false;
    }

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
static Polynomial polynomial_bigger(Polynomial p1, Polynomial p2)
{
    if(polynomial_is_null(p1) || polynomial_is_null(p2)) return NULL;

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
static Polynomial polynomial_smaller(Polynomial p1, Polynomial p2)
{
    if(polynomial_is_null(p1) || polynomial_is_null(p2)) return NULL;

    return (p1->degree > p2->degree) ? p2 : p1;
}

/**
 * Returns the zero polynomial, i.e. p(x) = 0.
 *
 * @return the zero polynomial
 */
static Polynomial polynomial_zero()
{
    return polynomial_new(0);
}

/**
 * Reduces a polynomial to its canonical form.
 *
 * @param[in] p the polynomial to be reduced
 */
static Polynomial polynomial_reduce(Polynomial p)
{
    if(polynomial_is_null(p)) return NULL;

    size_t degree = p->degree;

    while (degree > 0 && p->terms[degree] == 0.0) degree--;

    if(degree != p->degree)
        p->terms = (double *) realloc(p->terms, (degree+1) * sizeof (double));

    p->degree = degree;

    return p;
}
