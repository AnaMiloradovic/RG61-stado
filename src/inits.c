#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "colors.h"
#include "inits.h"
#include "callbacks.h"
#include "levels.h"
#include "draw_objects.h"
char Name[]= "Stado - sacuvaj sve ovce! 🐑";  //Naslov prozora - i ime igre :)
int ifJustSheeps = 1;
int NumOfSheeps, Level;
float minX,maxX,minZ,maxZ;
int closedSurfacesLX=0, closedSurfacesRX =0, closedSurfacesUZ=0, closedSurfacesDZ =0;  /* TODO: Broj 'jedinicnih povrsina' koje ogranicavaju ovce, redom, sleva, sdesna, s gornje strane i s donje strane.*/
int points = 0;   /* TODO: Bodovi i procenat zatvorene povrsine*/
int closedPercentOfMeadow = 0;
BALL Balls[MAX_NUM_OF_SHEEPS];
CLOSER Closer;


/* --- Konstante koje su tu samo radi citljivosti i mini-optimizacije, da se ne racunaju stalno */
const double GlobalXSize = 1.0/MEADOWDIMENSION_X;
const double GlobalYSize = 1.0/MEADOWDIMENSION_Y;
const double GlobalZSize = 1.0/MEADOWDIMENSION_Z;
/* --- */

/* HACK:  SURFACE ClosedSurfaces[MAX_POSSIBLE_CLOSED_SURFACES];  Ovde cuvamo informacije o svim delovima uspesno zatvorenih povrsina u igri
 Azurirace se svaki put ako igrac uspe da zatvori odredjeni deo slobodne povrsine, a neophodan je zbog iscrtavanja zatvorenih delova porvsine
   HACK: int NumOfClosedSurfaces = 0; Broj delova uspesno zatvorene povrsine
*/

/* HACK
//(Da se mozda i doradi) Informacije neophodne za samu igru -----
float PercentOfCoveredField = 0; //Procenat uspesno zatvorene povrsine (kada se dodje do trazenog procenta, u zavisnosti od broja ovaca, igrac je pobedio)
float PercentNeedForWin;          // Neophodan procenat za pobedu (utvrdjuje se na pocetku igre, kada se ucitaju nivo i broj ovaca)
// -----
 */

void graphicsInitialization()
{
    glutInitWindowSize(WINDOW_WIDTH,WINDOW_HEIGHT); //Kreiranje prozora i neophodne inicijalizacije
    glutInitWindowPosition(WINDOW_POSITION_X,WINDOW_POSITION_Y);
    glutCreateWindow(Name);
    glutDisplayFunc(onDisplayFunction);  //Registrujemo callback funkcije
    glutReshapeFunc(onReshapeFunction);
    glutKeyboardFunc(onKeyboardFunction);
    glutSpecialFunc(onKeyboardSpecialFunc);
    glClearColor(COLOR_SKY_R,COLOR_SKY_G,COLOR_SKY_B,0);  //Boja neba - pre nego sto postavimo osvetljenje
    glEnable(GL_DEPTH_TEST);
    return;
}

void gameDataInitialization()
{
   int toContinue=0; //Indikator korektnog unosa
   printf("Izaberite nivo(1-10) i broj ovaca(1-5):\n\n-> ");
   while(!toContinue)  //Dok se ne unesu korektni podaci
   {
      scanf("%d%d",&Level,&NumOfSheeps);
      if(Level < 1 || Level > 10 || NumOfSheeps < 1 || NumOfSheeps > MAX_NUM_OF_SHEEPS)
          printf("Niste korektno uneli nivo i broj ovaca:\n-> ");
      else toContinue=1; //Podaci su korektno uneti, mozemo iz petlje
   }

}

void initialPos()
{
    static int isCalled = 0;  //Indikator da li je ova funkcija vec pozivana
    if(isCalled)
        return;
    srand(time(NULL)); //Inicijalizujemo nas random generator
    int i;  
    float v; // Ukupna brzina kretanja kugle(bice ista za sve)
    /* TODO: Srediti brzine! */
    switch(Level)
    {
        case 1:
            v = FAST_LEVEL_1;
            break;
        case 2:
            v = FAST_LEVEL_2;
            break;
        case 3:
            v = FAST_LEVEL_3;
            break;
        case 4:
            v = FAST_LEVEL_4;
            break;
        case 5:
            v = FAST_LEVEL_5;
            break;
    }
    
    minX = -1;    // Inicijalizuju se dimenzije slobodne povrsine za ovce
    maxX = 1;
    minZ = -1;
    maxZ = 1;

    for(i=0;i<NumOfSheeps;i++)
    {
       /* Ovakvim racunanjem se obezbedjujemo da sigurno ne dodje do kolizije pri inicijalnom iscrtavanju objekata  */
       Balls[i].pX = ((float)rand()/RAND_MAX)*(1.0/NumOfSheeps)*(2*(1.0*RADIUS/MEADOWDIMENSION_X - 1))+(-RADIUS*GlobalXSize + 1 + ((float)i/NumOfSheeps)* ( 2 * ( RADIUS * GlobalXSize -1 )));
       Balls[i].pZ = ((float)rand()/RAND_MAX)*(1.0/NumOfSheeps)*(2*(1.0*RADIUS/MEADOWDIMENSION_Z - 1))+(-1.0*RADIUS/MEADOWDIMENSION_Z+1+((float)i/NumOfSheeps)*(2*(1.0*RADIUS/MEADOWDIMENSION_Z-1)));
       Balls[i].angle = 0;
       if(ifJustSheeps)
       {
           Balls[i].angle = ((float)rand()/RAND_MAX)*2*PI;
       }
       Balls[i].alfa= ((float)rand()/RAND_MAX)*2*PI;
       Balls[i].vX = v*cos(Balls[i].alfa);
       Balls[i].vZ = v*sin(Balls[i].alfa);
       Balls[i].w_angle = v/RADIUS;
       /*if(ifJustSheeps)
           Balls[i].w_angle = PI/6;*/
    }

    Closer.pX = 0;
    Closer.pY = 10;
    Closer.pZ = 1;

    Closer.vZ = 1/(float) 50;  /*TODO: Da li prepoloviti brzinu? */
    Closer.vX = 2 * Closer.vZ;

    isCalled =1; //Posle ovoga se vise nece inicijalizovati podaci.
}