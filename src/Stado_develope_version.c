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
/* ********************* */
extern int hit;
extern char curDir;
extern int timePast;

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
    glClearColor(COLOR_SKY_R - (((float) COLOR_SKY_R - DARK_BLUE_R) / TIME_OUT) * timePast,
                 COLOR_SKY_G - (((float) COLOR_SKY_G - DARK_BLUE_G) / TIME_OUT) * timePast,
                 COLOR_SKY_B - (((float) COLOR_SKY_B - DARK_BLUE_B) / TIME_OUT) * timePast,0); //Boja neba - pre nego sto postavimo osvetljenje
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Cistimo sadrzaj prozora
    glEnable(GL_LIGHTING); //Postavljamo osvetljenje
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    /*
    //glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);
    //drawClouds();
     */
    setSunLight();

    glMatrixMode(GL_MODELVIEW);  //Podesavamo scenu i pogled
    glLoadIdentity();
    if(Closer.pZ*MEADOWDIMENSION_Z <= 0)
        gluLookAt(0,5,Closer.pZ*MEADOWDIMENSION_Z+13,Closer.pX*MEADOWDIMENSION_X/10.0,Closer.pY*MEADOWDIMENSION_Y+1,Closer.pZ*MEADOWDIMENSION_Z,0,1,0);
    else
    gluLookAt(0,5,13,Closer.pX*MEADOWDIMENSION_X/10.0,Closer.pY*MEADOWDIMENSION_Y+1,Closer.pZ*MEADOWDIMENSION_Z,0,1,0); // Pogled(posmatraceva pozicija, tacka pogleda, 'vektor nagore')

    setMeadowMaterial(); // Postavljamo materijal na livadu
    glPushMatrix(); // Iscrtavamo livadu(glavni teren) gde ce nam trcati ovce.
    drawMeadow(); // Ne radimo glPopMatrix(), jer hocemo da nam na dalje koordinatni sistem bude
                 // vezan za teren.
    glPushMatrix();
       drawBlocks();
    glPopMatrix();

    initialPos();   // Inicijalizujemo pocetne pozicije kugli
    drawObjects();

    glPushMatrix();
    setCloserMaterial(); 
    drawCloser();  // Iscrtavamo valjak
    glPopMatrix();

     if(hit)      /* TODO USLOVI */
     {
         drawHitting();
         drawHittingPath();
     }

    setMeadowMaterial();
    glPushMatrix();
       drawBlocks();
    glPopMatrix();
    glPopMatrix();


    if(timePast == TIME_OUT)
    {
        printf("Isteklo vreme - kraj igre \n");
        exit(EXIT_SUCCESS);
    }
    if(on_going)
    {
       glutTimerFunc(TIMER_INTERVAL,rollingBalls,TIMER_ID_ROLLING);
       timePast++;
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
