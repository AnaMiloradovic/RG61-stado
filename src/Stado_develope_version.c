#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//TODO: Kada budemo imali implementaciju iscrtavanja ovce i funkcionalnosti njenog kretanja,
//      sve neophodne funkcije i konstante ce biti deklarisane u ovom fajlu.
//#include "sheep.h"
#include "colors.h" //Ovde cuvamo boje koje koristimo u igri
#include "inits.h" //Ovde nam se nalaze deklaracije inicijalizacionih funkcija
#include "callbacks.h" //Callback funckije
#include "draw_objects.h" //Funckije za iscrtavanje objekata
#include "light_and_materials.h" //Funkcije za podesavanje osvetljenja i postavljanje materijala
#include "levels.h" //Datoteka sa brzinama po nivoima
#include "game_dynamics.h" //Funckije za kretanja u igri

extern BALL Balls[];
extern CLOSER Closer;
extern char Name[];
extern int NumOfSheeps, Level;
extern int on_going;
extern int closing;
// NOTE: Za sada cemo ovde apstrahovati ovce belim kuglama koje se kotrljaju po
// terenu, pa cemo mozda u nekoj od poslednjih iteracija zameniti ovcama.
int main(int argc, char* argv[])
{
   gameDataInitialization();
   glutInit(&argc,argv);  //Neophodna inicijalizacija glut-a
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

   graphicsInitialization();
   glutMainLoop();
   return 0;
}

void onDisplayFunction()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Cistimo sadrzaj prozora
    glEnable(GL_LIGHTING); //Postavljamo osvetljenje
    glEnable(GL_LIGHT0);
    
    drawClouds();
    setSunLight();

    glMatrixMode(GL_MODELVIEW);  //Podesavamo scenu i pogled
    glLoadIdentity();        // Cistimo matricu od prethodnog smeca.
    gluLookAt(0,5,13,0,0,0,0,1,0); // Pogled(posmatraceva pozicija, tacka pogleda, 'vektor nagore')

    setMeadowMaterial(); // Postavljamo materijal na livadu
    glPushMatrix(); // Iscrtavamo livadu(glavni teren) gde ce nam trcati ovce.
    drawMeadow(); // Ne radimo glPopMatrix(), jer hocemo da nam na dalje koordinatni sistem bude
                                                                // vezan za teren.
    setBallMaterial(); // Postavljamo materijal na kugle
    initialPos();   // Inicijalizujemo pocetne pozicije kugli
    glPushMatrix();
    drawBalls();   // Iscrtavamo kugle
    glPopMatrix();
    
    glPushMatrix();
    setCloserMaterial(); 
    drawCloser();  // Iscrtavamo valjak
    glPopMatrix();
    
    
    glPopMatrix();
    if(on_going)
    {
       glutTimerFunc(TIMER_INTERVAL,rollingBalls,TIMER_ID_ROLLING);
    }
    glutSwapBuffers();
}
    // TODO: U reshape-u postavi da vratis projekciju
