#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "sheep.h"
#include "colors.h" //Ovde cuvamo boje koje koristimo u igri
#include "inits.h" //Ovde nam se nalaze deklaracije inicijalizacionih funkcija
#include "callbacks.h" //Callback funckije
#include "draw_objects.h" //Funckije za iscrtavanje objekata
#include "light_and_materials.h" //Funkcije za podesavanje osvetljenja i postavljanje materijala
#include "levels.h" //Datoteka sa brzinama po nivoima
#include "game_dynamics.h" //Funckije za kretanja u igri

const char pause[] = "Pauza";
#define PAUZE_LEN (5)
extern int ifJustSheeps;
extern BALL Balls[];
extern CLOSER Closer;
extern char Name[];
extern int NumOfSheeps, Level;
extern float minX,maxX,minZ,maxZ;
extern int on_going;
extern int closing;
extern unsigned char moving;

int main(int argc, char* argv[])
{
   if(argc > 1 && argv[1][0] == '-' && argv[1][1] == 'k')
       ifJustSheeps = 0;
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
    glEnable(GL_NORMALIZE);
    //glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);
    drawClouds();
    setSunLight();

    glMatrixMode(GL_MODELVIEW);  //Podesavamo scenu i pogled
    glLoadIdentity();        // Cistimo matricu od prethodnog smeca.
    gluLookAt(0,5,13,0,0,0,0,1,0); // Pogled(posmatraceva pozicija, tacka pogleda, 'vektor nagore')

    setMeadowMaterial(); // Postavljamo materijal na livadu
    glPushMatrix(); // Iscrtavamo livadu(glavni teren) gde ce nam trcati ovce.
    drawMeadow(); // Ne radimo glPopMatrix(), jer hocemo da nam na dalje koordinatni sistem bude
                                                                // vezan za teren.
    
    initialPos();   // Inicijalizujemo pocetne pozicije kugli
    
    if(ifJustSheeps)
        drawSheeps();
    
    else
    {
    glPushMatrix();
    drawBalls();   // Iscrtavamo kugle
    glPopMatrix();
    }
    
    glPushMatrix();
    setCloserMaterial(); 
    drawCloser();  // Iscrtavamo valjak
    glPopMatrix();
    
    if(closing)
        tryToClose();

    drawClosedSurfaces();
    glPopMatrix();
    if(on_going)
    {
       glutTimerFunc(TIMER_INTERVAL,rollingBalls,TIMER_ID_ROLLING);
    }

    else
    {
        glPushMatrix();
        setLetters();
        glRasterPos3f(-0.5,1.5,1.5);
        for(int i=0;i<PAUZE_LEN;i++)
           glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,pause[i]);
    }
    
    glutSwapBuffers();
}
