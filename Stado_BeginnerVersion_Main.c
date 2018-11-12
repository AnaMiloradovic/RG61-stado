#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//TODO: Kada budemo imali implementaciju iscrtavanja ovce i funkcionalnosti njenog kretanja,
//      sve neophodne funkcije i konstante ce biti deklarisane u ovom fajlu.
//#include "sheep.h"
#include "colors.h" //Ovde cuvamo boje koje koristimo u igri
#include "inits.h" //Ovde nam se nalaze deklaracije inicijalizacionih funkcija

char Name[]= "Stado - sacuvaj sve ovce! 🐑"; //Naslov prozora - i ime igre :)
int NumOfSheeps, Level;  //Ovde cuvamo broj ovaca, odnosno nivo igre
int width,height;

//TODO: static void drawClouds();             //Crtanje oblaka
static void onDisplayFunction();      // Funkcija za obradu dogadjaja
//TODO: static void onKeyboardFunction();  // Funkcija koja reaguje na dogadjaje sa tastature
static void onReshapeFunction(int width, int height);   // Funkcija koja reaguje na promene velicine prozora
int main(int argc, char* argv[])
{
   gameDataInitialization();
   
   glutInit(&argc,argv);  //Neophodna inicijalizacija glut-a
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
   
   graphicsInitialization();
   glutMainLoop();
   return 0;
}

void onDisplayFunction()
{
    glClear(GL_COLOR_BUFFER_BIT);
    //drawClouds();
    glutSwapBuffers();
}

void onReshapeFunction(int w, int h)
{
    width=w;
    height=h;
}
    

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