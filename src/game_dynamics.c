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
extern int NumOfSheeps;

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
    else if(Balls[i].angle < 360)
        Balls[i].angle += 360;
    colissionDetection();
    
    /*pozX += vX;
    if(pozX + 1 >= 5/0.5 || pozX  - 1 <= -5/0.5) {

        vX = -vX;
        d_angle = 360 -d_angle;
    }
    pozZ += vZ;
    if(pozZ + 1 >= 10/0.5 || pozZ  - 1 <= -10/0.5) {
        vZ = -vZ;
        d_angle = 360 -d_angle;
    }
    angle += d_angle;
    if(angle > 360)
        angle -= 360;
    if(angle < 0)
        angle += 360;*/
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
    
    int i,colission[]={1,1,1,1,1};
    for(i=0;i<NumOfSheeps;i++)
    {
        if(Balls[i].pX + 1 >= Balls[i].maxX || Balls[i].pX  - 1 <= Balls[i].minX)
        {
            Balls[i].vX = -Balls[i].vX;
            Balls[i].w_angle = - Balls[i].w_angle;
        }
        if(Balls[i].pZ + 1 >= Balls[i].maxZ || Balls[i].pZ  - 1 <= Balls[i].minZ)
        {
            Balls[i].vZ = -Balls[i].vZ;
            Balls[i].w_angle = - Balls[i].w_angle;
        }
    }
}
//         if(colission[i])
//         {
//             int j;
//             for(j=i+1;j<NumOfSheeps;j++)
//                if(colission[j])
//                 {
//                     if(fabs(Balls[i].pX-Balls[j].pX) <= 2)
//                     {
//                         Balls[i].vX = -Balls[i].vX;
//                         Balls[i].w_angle = - Balls[i].w_angle;
//                         Balls[j].vX = -Balls[j].vX;
//                         Balls[j].w_angle = - Balls[j].w_angle;
//                         colission[j] = 0;
//                     }
//             
//          }
// 
//             for(j=i+1;j<NumOfSheeps;j++)
//                if(colission[j])
//                if(fabs(Balls[i].pZ-Balls[j].pZ) <= 2)
//                     {
//                         Balls[i].vZ = -Balls[i].vZ;
//                         Balls[i].w_angle = - Balls[i].w_angle;
//                         Balls[j].vZ = -Balls[j].vZ;
//                         Balls[j].w_angle = - Balls[j].w_angle;
//                         colission[j] = 0;
//                     }
//             colission[i]=0;
//         }
// 
//     }
//}
    
//     int colission[NumOfSheeps]; //Indikatorski niz koji cemo koristiti pri ispitivanju kolizije
//     for(i=0;i<NumOfSheeps;i++)  //Ispitujemo prvo donji levi kvadrant
//     {
//         if(Balls[i].pX + 1 < 0 && Balls[i].pZ + 1 < 0)
//             colission[i] = TO_CHECK;
//         else colission[i] = NOT_TO_CHECK;
//     }
//     
//     colissionInField(colission); // Zatim medju kuglama u kvadrantu ispitamo koliziju
//     // Analogno radimo i za ostale kvadrante
//     
//     for(i=0;i<NumOfSheeps;i++)  // Gornji levi kvadrant
//     {
//         if(Balls[i].pX + 1 < 0 && Balls[i].pZ + 1 >= 0)
//             colission[i] = TO_CHECK;
//         else colission[i] = NOT_TO_CHECK;
//     }
//     
//     colissionInField(colission);
//     
//         for(i=0;i<NumOfSheeps;i++)  // Donji desni kvadrant
//     {
//         if(Balls[i].pX + 1 >= 0 && Balls[i].pZ + 1 < 0)
//             colission[i] = TO_CHECK;
//         else colission[i] = NOT_TO_CHECK;
//     }
//     
//     colissionInField(colission);
//     
//     for(i=0;i<NumOfSheeps;i++)  //Gornjii desni kvadrant
//     {
//         if(Balls[i].pX + 1 >= 0 && Balls[i].pZ + 1 >= 0)
//             colission[i] = TO_CHECK;
//         else colission[i] = NOT_TO_CHECK;
//     }
//     
//     colissionInField(colission);



// void colissionInField(int colission[])
// {
//         int i;
//         for(i=0;i<NumOfSheeps;i++)
//         if(colission[i])
//         {
//             int j;
//             for(j=i;j<NumOfSheeps;j++)
//             {
//                 if(colission[j])
//                 {
//                     if(fabs(Balls[i].pX-Balls[j].pX) <= 2)
//                     {
//                         Balls[i].vX = -Balls[i].vX;
//                         Balls[i].w_angle = - Balls[i].w_angle;
//                         Balls[j].vX = -Balls[j].vX;
//                         Balls[j].w_angle = - Balls[j].w_angle;
//                     }
//                     if(fabs(Balls[i].pZ-Balls[j].pZ) <= 2)
//                     {
//                         Balls[i].vZ = -Balls[i].vZ;
//                         Balls[i].w_angle = - Balls[i].w_angle;
//                         Balls[j].vZ = -Balls[j].vZ;
//                         Balls[j].w_angle = - Balls[j].w_angle;
//                     }
//                 }
//                 colission[j] = NOT_TO_CHECK;
//             }
//             colission[i] = NOT_TO_CHECK;
//         }
// }