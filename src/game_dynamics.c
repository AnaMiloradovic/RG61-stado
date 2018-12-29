#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
//TODO: Kada budemo imali implementaciju iscrtavanja ovce i funkcionalnosti njenog kretanja,
//      sve neophodne funkcije i konstante ce biti deklarisane u ovom fajlu.
//#include "sheep.h"
#include "colors.h" //Ovde cuvamo boje koje koristimo u igri
#include "inits.h" //Ovde nam se nalaze deklaracije inicijalizacionih funkcija
#include "callbacks.h" //Callback funckije
#include "draw_objects.h" //Funckije za iscrtavanje objekata
#include "light_and_materials.h" //Funkcije za podesavanje osvetljenja i postavljanje materijala
#include "game_dynamics.h" //Funckije za kretanja u igri

extern BALL Balls[];
extern CLOSER Closer;
extern int NumOfSheeps;
int on_going = 1;
int closing = 0;

void rollingBalls(int timer_id)
{
    if(timer_id != TIMER_ID_ROLLING)
        return;
    int i;
    for(i=0;i<NumOfSheeps;i++)
    {
        Balls[i].pX += Balls[i].vX;
        Balls[i].pZ += Balls[i].vZ;
        Balls[i].angle += Balls[i].w_angle;
    }
    
    if(Balls[i].angle > 360)
       Balls[i].angle -= 360;
    else if(Balls[i].angle < -360)
       Balls[i].angle += 360;
    colissionDetection();



    glutPostRedisplay();
}

void colissionDetection()
{
    // Imamo veci broj objekata na terenu, i kada bismo ponaosob analizirali
    // da li ima sudara izmedju svake dve kugle, to bi uzimalo previse
    // vremena i to pri svakom pozivu(vremenska slozenost O(n^2), tj.
    // kvadratna je u odnosu na broj kugli. Zato moramo da primenimo
    // drugaciji pristup: Mozemo podeliti teren na 4 dela('kvadranta')
    // i u svakom od njih ispitamo koje od kugli su prisutne i onda medju
    // njima ispitivali prisustvo kolizije. U najgorem slucaju vremenska
    // slozenost bi ostala ista (imamo 5 kugli i sve idu u isti kvadrant),
    // ali takav slucaj je malo verovatan i statisticki se retko desava.
    // U prosecnom slucaju(koji je mnogo cesci), u svakom kvadrantu se nalaze
    // najvise dve kugle.
    // NAPOMENA: Ovo je slucaj kada se jos uvek nalazi relativno mali broj kugli.
    // Kada bismo generalizovali situaciju na mnogo veci broj kugli(recimo reda velicine 100)
    // u ovoj strategiji bismo isli dublje sa podelom. Teren bismo delili sve dok u nekom
    // segmentu ne ostanu do najvise dve kugle.
    
    int i,j,colission[]={TO_CHECK,TO_CHECK,TO_CHECK,TO_CHECK,TO_CHECK};
    for(i=0;i<NumOfSheeps;i++)
    {
        if(Balls[i].pX + ((float)RADIUS/MEADOWDIMENSION_X) >= Balls[i].maxX || Balls[i].pX  - ((float)RADIUS/MEADOWDIMENSION_X) <= Balls[i].minX)
        {
            Balls[i].vX = -Balls[i].vX;
            Balls[i].w_angle = - Balls[i].w_angle;
            colission[i] = NOT_TO_CHECK;
        }
        if(Balls[i].pZ + ((float)RADIUS/MEADOWDIMENSION_Z) >= Balls[i].maxZ || Balls[i].pZ  - ((float)RADIUS/MEADOWDIMENSION_Z) <= Balls[i].minZ)
        {
            Balls[i].vZ = -Balls[i].vZ;
            Balls[i].w_angle = - Balls[i].w_angle;
            colission[i] = NOT_TO_CHECK;
        }
    }
    
    for(i=0;i<NumOfSheeps;i++)
        for(j=i+1;j<NumOfSheeps;j++)
        {
            if(MEADOWDIMENSION_X*MEADOWDIMENSION_X*(Balls[i].pX-Balls[j].pX)*(Balls[i].pX-Balls[j].pX) +
                 MEADOWDIMENSION_Z*MEADOWDIMENSION_Z*(Balls[i].pZ-Balls[j].pZ)*(Balls[i].pZ-Balls[j].pZ) - RADIUS*RADIUS*2*2 < EPSILON)  
            {
                if(colission[i])
                {
                   Balls[i].vX = -Balls[i].vX;
                   Balls[i].vZ = -Balls[i].vZ;
                   colission[i]= NOT_TO_CHECK;
                }
                if(colission[j])
                {
                   Balls[j].vX = -Balls[j].vX;
                   Balls[j].vZ = -Balls[j].vZ;
                   colission[j] = NOT_TO_CHECK;
                }
            }
        }
}
