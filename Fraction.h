#ifndef FRACTION_H_INCLUDED
#define FRACTION_H_INCLUDED



typedef struct Fraction_s Fraction;

Fraction  * NFRA_new_Fraction( long int numerateur ,long int denominateur);
Fraction  * NFRA_Fraction_copy                    (Fraction *p);
void NFRA_Fraction_destroy(Fraction **p);

double NFRA_Get_Value(Fraction *p);


void NFRA_Set_num(Fraction *p,long int num);
void NFRA_Set_deno(Fraction *p,long int deno);

long int NFRA_Get_num(Fraction *p);
long int NFRA_Get_deno(Fraction *p);

Fraction * NFRA_Get_Inverse(Fraction *F);
void NFRA_Optimise(Fraction **F);
int NFRA_check_null(Fraction *F);

/// Oprations

Fraction * NFRA_add(Fraction *F1, Fraction*F2);
Fraction * NFRA_soustract(Fraction *F1, Fraction*F2);
Fraction * NFRA_multiplication(Fraction *F1, Fraction*F2);
Fraction * NFRA_division(Fraction *F1, Fraction*F2);


char * NFRA_ToString(Fraction *F);

#endif // FRACTION_H_INCLUDED
