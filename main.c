#include <stdio.h>
#include <stdlib.h>
#include "Fraction.h"
#include "polynomial.h"
#include "FractionPolymon.h"


Polynomial * Lagrange(int n, int k);
Polynomial * fx;
Fraction ** tabFractionX,** tabFractionY ;
Polynomial * Calculate(int n);



int main()
{
    int nbr_points=1;
    long int a1,b1,a2,b2;

    ///   Allways the table of Fractions has the the order of the polynomial +1 as size for x^0 of course


/*
        // Testing Code

     Fraction *F__1= NFRA_new_Fraction(1,1);
    // Fraction *F__0= NFRA_new_Fraction(0,1);

     Fraction **tmp__tab = (Fraction**)malloc(3*sizeof(Fraction*));
    tmp__tab[0]= F__1;
    tmp__tab[1]= F__1;
    tmp__tab[2]= F__1;

    Polynomial * X = polynomial_new_tab(tmp__tab,2);/// P[x]=x²+x+1

    tmp__tab[0]=F__1;
    tmp__tab[1]= F__1;

    Polynomial *Y = polynomial_new_tab(tmp__tab,1); // p[x]=x+1


    Polynomial * Y_X = polynomial_subtract(Y,X);//<-

    Polynomial * X_t_Y = polynomial_add(X,Y);

      Polynomial * X_x_Y = polynomial_multiply(X,Y);

        Polynomial * X_x_3l4 = polynomial_multiply_by_constant(X, NFRA_new_Fraction(3,4));

        printf(" \n\n\n Polynomial s X = %s  \n\n  ", Poly_ToString(X));
            printf(" \n\n\n Polynomial s Y = %s  \n\n  ", Poly_ToString(Y));

    printf("  \n\n\n =>   Polynomial s X x 3/4 = %s  \n\n  ",Poly_ToString(X_x_3l4));
    exit(1);

*/




    do{
    printf(" \n\n\t Saisissez le nbr de points (>0) :  ");
    scanf(" %d",&nbr_points);
    }while (nbr_points<=0);


    tabFractionX = (Fraction **) malloc(nbr_points*sizeof(Fraction *));
    tabFractionY =(Fraction **) malloc(nbr_points*sizeof(Fraction *));

    if(!tabFractionX || !tabFractionY){
        printf(" \n\n\t  Erreur à la création du tableau : 7 ");
        exit(7);
        }

    printf("\n\n--------------------------------------\n\n\t Saisissez les coordonnées de chaque point sous format d'une fraction exemple x=3/4 : \n\n");
    for( int i=0; i<nbr_points; i++){

        printf("\n\n\n\t\t Point:%d ( x, y) ==> x= ",i);
        scanf("%ld/%ld",&a1,&b1);
        tabFractionX[i]= NFRA_new_Fraction(a1,b1);

        printf("\n\n\t\t Point:%d ( %ld/%ld, y) ==> y= ", i,a1,b1);
        scanf("%ld/%ld",&a2,&b2);
        tabFractionY[i]= NFRA_new_Fraction(a2,b2);
    }

    printf("\n\n\n\n\t\t Les points Saisies sont : ");

    for(int i=0;i<nbr_points;i++){
        if( NFRA_check_posi(tabFractionX[i]) )
            printf(" \n\n\t P%d :( %s,",i,NFRA_ToString(tabFractionX[i]));
        else
            printf(" \n\n\t P%d :( -%s,",i,NFRA_ToString(tabFractionX[i]));
        if( NFRA_check_posi(tabFractionY[i]) )
            printf(" %s) ",NFRA_ToString(tabFractionY[i]));
        else
            printf(" -%s) ",NFRA_ToString(tabFractionY[i]));
    }



    printf("\n\n\n\t ==> Interpolant : f(x) = %s \n\n\n\n", Poly_ToString( Calculate( nbr_points ) ) );


    return 0;
}


/*

void lala() {
 printf("Result =====> f(%4.2lf) = %5.4lf\n",question, Calculate(size, question));
}
*/

Polynomial* Lagrange(int n, int k)
{
 Fraction *F__1= NFRA_new_Fraction(1,1);
 Fraction *F__0= NFRA_new_Fraction(0,1);
 Fraction **tmp__tab = (Fraction**)malloc(2*sizeof(Fraction*));
 tmp__tab[0]= F__1;
 tmp__tab[1]= F__0;

 Polynomial * X = polynomial_new_tab(tmp__tab,1);/// P[x]=x


 tmp__tab[0] = F__1;
 Polynomial * result = polynomial_new_tab(tmp__tab,0);
 Polynomial * P__1 = polynomial_new_tab(tmp__tab,0);
Polynomial *tmp ;

 for (int i = 0; i < n; i++)
 {
 if (i!=k){
    Polynomial *top = polynomial_subtract(X, polynomial_multiply_by_constant(P__1,tabFractionX[i]) );
    Fraction *bottom = NFRA_soustract(tabFractionX[k] , tabFractionX[i]);
    Fraction *Inverse_bottom = NFRA_Get_Inverse(bottom);

    tmp = polynomial_multiply_by_constant(top,Inverse_bottom);
    result = polynomial_multiply(result,tmp);}
 }
 return result;
}

Polynomial * Calculate(int n)
{

    Fraction * F__0 = NFRA_new_Fraction(0,1);
    Fraction **tmp_tab= (Fraction **) malloc(1*sizeof(Fraction*));
    tmp_tab[0] = F__0;

    Polynomial *final__pol= polynomial_new_tab(tmp_tab,0); /// initiate this pol with 0
    Polynomial *L = (Polynomial *)malloc(sizeof(Polynomial*)) ;

    Polynomial *Fx__x__Lx = (Polynomial *)malloc(sizeof(Polynomial*)) ;
 for (int i = 0; i < n; i++)
 {
 L = Lagrange(n, i);
  // printf(" \n\n i:%d ==> p(x) = %s \n\n ",i,Poly_ToString(L));
 Fx__x__Lx = polynomial_multiply_by_constant(L,tabFractionY[i]);
 final__pol = polynomial_add(final__pol,Fx__x__Lx);
 }

 return final__pol;
}
