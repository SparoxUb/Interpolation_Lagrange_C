#include "Fraction.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

const int ENOUGH_NUMBER_SIZE=200;

struct Fraction_s
{
    long int numerateur;
    long int denominateur;
};



Fraction  * NFRA_new_Fraction( long int numerateur ,long int denominateur){

    if(denominateur==0)
        denominateur=1;

    Fraction *p = (Fraction *) malloc(sizeof(struct Fraction_s));
    if (!p) return NULL;

    p->numerateur = numerateur;
    p->denominateur = denominateur;

    NFRA_Optimise(&p);
    return p;
}

Fraction  * NFRA_Fraction_copy(Fraction *p){
    if (p == NULL) return NULL;

    Fraction *copy = NFRA_new_Fraction(p->numerateur,p->denominateur);
    if (!copy) return NULL;

    return copy;
}


void NFRA_Fraction_destroy(Fraction **p)
{
    if (*p == NULL) return;

    free(*p);
    *p = NULL;
}

double NFRA_Get_Value(Fraction *p){
    return p->numerateur/p->denominateur;
}


void NFRA_Set_num(Fraction *p,long int num){
p->numerateur=num;
NFRA_Optimise(&p);
}


void Set_deno(Fraction *p,long int deno){
    p->denominateur=deno;
    NFRA_Optimise(&p);
}


long int NFRA_Get_num(Fraction *p){
return p->numerateur;
}

long int NFRA_Get_deno(Fraction *p){
return p->denominateur;
}

Fraction * NFRA_Get_Inverse(Fraction *F){
    Fraction * tmp = NFRA_new_Fraction(F->denominateur,F->numerateur);
    NFRA_Optimise(&tmp);
    return tmp;
}


Fraction * NFRA_add(Fraction *F1, Fraction*F2){
    Fraction *somme =NULL;
    if (F1 == NULL || F2==NULL) return NULL;

    if(F1->denominateur == F2->denominateur){
       somme = NFRA_new_Fraction( F1->numerateur+F2->numerateur , F1->denominateur);
    }else{
        somme = NFRA_new_Fraction( (  (F1->numerateur ) * (F2->denominateur) )+ ( (F2->numerateur) * (F1->denominateur) ) , (F1->denominateur) * (F2->denominateur) );
    }

    if (!somme) return NULL;
    NFRA_Optimise(&somme);

    return somme;
}


Fraction * NFRA_soustract(Fraction *F1, Fraction*F2){


    Fraction *somme;
    long int numerateur,denominateur;

    if (F1 == NULL || F2==NULL) return NULL;

    if(F1->denominateur == F2->denominateur){
       numerateur = NFRA_Get_num(F1) - NFRA_Get_num(F2) ;
       denominateur = F1->denominateur ;

    }else{
        numerateur = ( F1->numerateur*F2->denominateur ) - ( F2->numerateur*F1->denominateur ) ;
        denominateur = ( (F1->denominateur ) * (F2->denominateur) );
    }

    somme = NFRA_new_Fraction( numerateur, denominateur );
    if (!somme) return NULL;

    NFRA_Optimise(&somme);

    return somme;
}


Fraction * NFRA_multiplication(Fraction *F1, Fraction*F2){
    Fraction *mult =NULL;
    if (F1 == NULL || F2==NULL) return NULL;

    mult = NFRA_new_Fraction( F1->numerateur * F2->numerateur , F1->denominateur * F2->denominateur);

    if (!mult) return NULL;
    NFRA_Optimise(&mult);

    return mult;
}

Fraction * NFRA_division(Fraction *F1, Fraction*F2){

    Fraction *res =NULL;
    if (F1 == NULL || F2==NULL) return NULL;

    res = NFRA_multiplication( F1, NFRA_Get_Inverse(F2) );

    if (!res) return NULL;

    return res;
}

int Myisspace(char s){
    if( (s)==' ')
        return 1;
    return false;
}

char *ltrim(char *s)
{
    while(Myisspace(*s)) s++;
    return s;
}

int NFRA_checkSingle(Fraction *F){
    if( (F->numerateur==1 || F->numerateur==-1) && F->denominateur==1 )
        return 1;
    return 0;
}

char *rtrim(char *s)
{
    char* back = s + strlen(s);
    while(Myisspace(*--back));
    *(back+1) = '\0';
    return s;
}

char *trim(char *s)
{
    return rtrim(ltrim(s));
}

char * NFRA_ToString(Fraction *F){
    char tmp[200];

    if(NFRA_Get_num(F)==0){
        sprintf( tmp,"0");
    }
    else{
        if(NFRA_Get_num(F)<0)
            sprintf( tmp,"%ld", - NFRA_Get_num(F) );
        else
            sprintf( tmp,"%ld", NFRA_Get_num(F) );


        if( (F->denominateur)!=1 )
            sprintf( tmp,"%s/%ld",tmp, NFRA_Get_deno(F));
    }
    return trim(tmp);
}

bool NFRA_equal (Fraction *F1,Fraction *F2){
    if(F1->denominateur!=F2->denominateur)
        return false;
    if(F1->numerateur!=F2->numerateur)
        return false;
    return true;
}


void NFRA_Optimise(Fraction **F){

    //## TODO
    if( ( (*F)->numerateur%(*F)->denominateur ==0 ) ){
        (*F)->numerateur /= (*F)->denominateur;
        (*F)->denominateur=1;
        }
    if( (*F)->numerateur<0 && (*F)->denominateur<0 )
    {
        (*F)->numerateur= - (*F)->numerateur;
        (*F)->denominateur= - (*F)->denominateur;
    }

    if((*F)->denominateur<0 && (*F)->numerateur>0){
        (*F)->numerateur= - (*F)->numerateur;
        (*F)->denominateur= - (*F)->denominateur;
    }

}

int NFRA_check_null(Fraction *F){
    if( NFRA_Get_num(F) == 0)
        return 1;
    return 0;
}

int NFRA_check_posi(Fraction *F){
    if( F->numerateur * F->denominateur >=0)
        return 1;
    return 0;
}


