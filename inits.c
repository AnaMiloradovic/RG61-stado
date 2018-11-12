#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "inits.h"
#include "colors.h"
//U ovom fajlu su nam definicije inicijalizacionih funkcija igre.

void gameDataInitialization()
{
   int toContinue=0; //Indikator korektnog unosa
   printf("Izaberite nivo(1-10) i broj ovaca(1-5):\n\n-> ");
   while(!toContinue)  //Dok se ne unesu korektni podaci
   {
      scanf("%d%d",&Level,&NumOfSheeps);
      if(Level < 1 || Level > 10 || NumOfSheeps < 1 || NumOfSheeps > 5)
          printf("Niste korektno uneli nivo i broj ovaca:\n-> ");
      else toContinue=1; //Podaci su korektno uneti, mozemo iz petlje
   }
}

void graphicsInitialization()
{
    glutInitWindowSize(WINDOW_WIDTH,WINDOW_HEIGHT); //Kreiranje prozora i neophodne inicijalizacije
    glutInitWindowPosition(WINDOW_POSITION_X,WINDOW_POSITION_Y);
    glutCreateWindow(Name);
    
    glutDisplayFunc(onDisplayFunction);  //Registrujemo callback
    glClearColor(COLOR_SKY_R,COLOR_SKY_G,COLOR_SKY_B,0);  //Boja neba - pre nego sto postavimo osvetljenje
    return;
}