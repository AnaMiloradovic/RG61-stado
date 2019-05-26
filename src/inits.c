/* Izvorna datoteka sa implementacijama inicijalizacionih funkcija. */
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "inits.h"
#include "callbacks.h" /* Neophodna, jer se ovde registruju callback funkcije. */
#include "levels.h"    /* U ovom fajlu se nalaze podaci o brzinama koji se ovde inicijalizuju. */
char Name[]= "Stado - sacuvaj sve ovce! 🐑";  /* Naslov prozora - i ime igre :)   */

int NumOfSheeps, Level; /* Broj ovaca i izabrani nivo. */
float minX,maxX,minZ,maxZ; /* Granice X i Z koordinata u okviru kojih se nalazi slobodna povrsina za ovce. */
BALL Balls[MAX_NUM_OF_SHEEPS]; /* Niz ovcica u igri. Naziv ostao, jer je igra u pocetku razvijana na kuglama. */
CLOSER Closer;              /* Objekat koji zatvara. */
unsigned int timePast = 0; /* Dosadasnji broj pozivanja funkcije za iscrtavanje cele scene. */
unsigned int pointsWon = 0; /* Osvojeni poeni u igri. */
float coveredPercent = 0.0; /* Pokriven procenat slobodne povrsine. */
struct tm* today = NULL;    /* Struktura koja cuva tekuci dan. */

/* --- Jedinice u svetskom koordinatnom sistemu. Vise informacija u falu draw_objects.c. Vrednosti cuvamo u konstantama, umesto da definisemo makroe, da se ne bi stalno racunali isti kolicnici. */
const double GlobalXSize = 1.0/MEADOWDIMENSION_X;
const double GlobalYSize = 1.0/MEADOWDIMENSION_Y;
const double GlobalZSize = 1.0/MEADOWDIMENSION_Z;
/* --- */

void graphicsInitialization() /* Funkcija za graficke inicijalizacije. */
{
    glutInitWindowSize(WINDOW_WIDTH,WINDOW_HEIGHT); /* Prozor i njegova podesavanja. */
    glutInitWindowPosition(WINDOW_POSITION_X,WINDOW_POSITION_Y);
    glutCreateWindow(Name);

    glutDisplayFunc(onDisplayFunction);  /* Registracija callback funkcija. */
    glutReshapeFunc(onReshapeFunction);
    glutKeyboardFunc(onKeyboardFunction);
    glutSpecialFunc(onKeyboardSpecialFunc);
    return;
}

void gameDataInitialization() /* Funkcija za inicijalizaciju kljucnih podataka o igri. */
{
   int toContinue=0; /* Indikator korektnog unosa */
   printf("Izaberite nivo(1-5) i broj ovaca(1-5):\n\n-> ");
   while(!toContinue)  /* Dok se ne unesu korektni podaci */
   {
      scanf("%d%d",&Level,&NumOfSheeps);
      if(Level < 1 || Level > 10 || NumOfSheeps < 1 || NumOfSheeps > MAX_NUM_OF_SHEEPS)
          printf("Niste korektno uneli nivo i broj ovaca:\n-> ");
      else toContinue=1; /*Podaci su korektno uneti, mozemo iz petlje */
   }

    time_t now = time(NULL);
    today = localtime(&now);
    if(today == NULL)
       printf("Greska pri ucitavanju danasnjeg dana. \n");
    else if(today->tm_mon >= 5 && today->tm_mon <= 8)
       printf("Letnje racunanje vremena - 50%% duza partija :)\n");
    else
       printf("Default-no racunanje vremena - trajanje partije 75 sekundi \n");
}

void initialPos() /* Inicijalizacija pocetne pozicije kose, pocetnih pozicija ovaca, njihovih brzina i slobodne povrsine za trcanje. */
{
    static int isCalled = 0;  /* Indikator da li je ova funkcija vec pozivana */
    if(isCalled)
        return;
    srand(time(NULL)); /* Inicijalizujemo nas random generator. */
    int i;  
    float v; /* Ukupna brzina kretanja ovce. Ista je za sve ovce na terenu. */
    
                  /* U zavisnosti od nivoa podesavamo brzinu ovcica. */
    switch(Level) /* Necemo na svakom nivou povecavati brzinu vec cemo na nekim smanjivati dozvoljen procenat slobodne povrsine za pobedu. */
    {
        case 1:
            v = FAST_LEVEL_1;
            break;
        case 2:
        case 3:
            v = FAST_LEVEL_2;
            break;
        case 4:
        case 5:
            v = FAST_LEVEL_4;
            break;
    }
    
    minX = -1;    /* Inicijalizuju se pocetne dimenzije slobodne povrsine za ovce. */
    maxX = 1;
    minZ = -1;
    maxZ = 1;

    for(i=0;i<NumOfSheeps;i++) /* Inicijalizujemo pocetne pozicije ovaca. */
    {
       /* Ovakvim racunanjem se obezbedjujemo da sigurno ne dodje do kolizije pri inicijalnom iscrtavanju objekata  */
       Balls[i].pX = ((float)rand()/RAND_MAX)*(1.0/NumOfSheeps)*(2*(1.0*RADIUS/MEADOWDIMENSION_X - 1))+(-RADIUS*GlobalXSize + 1 + ((float)i/NumOfSheeps)* ( 2 * ( RADIUS * GlobalXSize -1 )));
       Balls[i].pZ = ((float)rand()/RAND_MAX)*(1.0/NumOfSheeps)*(2*(1.0*RADIUS/MEADOWDIMENSION_Z - 1))+(-1.0*RADIUS/MEADOWDIMENSION_Z+1+((float)i/NumOfSheeps)*(2*(1.0*RADIUS/MEADOWDIMENSION_Z-1)));

       Balls[i].alfa= ((float)rand()/RAND_MAX)*2*PI; /* I odnos izmedu razlicith koordinata brzine ce biti nasumican. */
       Balls[i].vX = v*cos(Balls[i].alfa);
       Balls[i].vZ = v*sin(Balls[i].alfa);
    }

    Closer.pX = 0.0f; /* Inicijalizujemo pocetnu poziciju i brzinu zatvarajuceg objekta. */
    Closer.pY = 10;
    Closer.pZ = 1.0f;

    Closer.vZ = 1/(float) 50;  
    Closer.vX = (float) 2 * Closer.vZ;

    isCalled =1; /* Posle ovoga se vise nece inicijalizovati podaci. */
}