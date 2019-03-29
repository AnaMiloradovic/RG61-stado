#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "colors.h" //Ovde cuvamo boje koje koristimo u igri
#include "inits.h" //Ovde nam se nalaze deklaracije inicijalizacionih funkcija
#include "callbacks.h" //Callback funckije
#include "draw_objects.h"
#include "light_and_materials.h"
#include "sheep.h"

extern BALL Balls[];
extern CLOSER Closer;
/* HACK: extern SURFACE ClosedSurfaces[MAX_POSSIBLE_CLOSED_SURFACES];
 HACK: extern int NumOfClosedSurfaces;   */
extern int NumOfSheeps, Level, ifJustSheeps;
extern int on_going;
extern float minX,maxX,minZ,maxZ;

/*--- Jedinice u svetskom koordinatnom sistemu */
extern double GlobalXSize;
extern double GlobalYSize;
extern double GlobalZSize;
/*---   */


void drawMeadow()
{
    glScalef(MEADOWDIMENSION_X, MEADOWDIMENSION_Y, MEADOWDIMENSION_Z);
    glutSolidCube(2);

    glPushMatrix();
    glTranslatef(1 + 0.5 * 0.5*GlobalXSize,0,0); /*TODO: Da se iskomentarise*/
    glScalef(0.5 * GlobalXSize,1 * GlobalYSize, 2 * (1 + 0.5*GlobalZSize) );
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(- 1 - 0.5 * 0.5*GlobalXSize,0,0); /*TODO: Da se iskomentarise*/
    glScalef(0.5 * GlobalXSize, GlobalYSize , 2 * (1 + 0.5*GlobalZSize) );
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,0,1 + 0.5 * 0.5*GlobalZSize); /*TODO: Da se iskomentarise*/
    glScalef(2 * (1 + 0.5 * GlobalXSize), GlobalYSize , 0.5 * GlobalZSize);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,0, - 1 - 0.5 * 0.5 * GlobalZSize); /*TODO: Da se iskomentarise*/
    glScalef(2 * (1 + 0.5 * GlobalXSize),GlobalYSize, 0.5 * GlobalZSize);
    glutSolidCube(1);
    glPopMatrix();
}
/*
void drawBalls()
{
    int i;
    double raiseBehindMeadow = 1+ ((float)RADIUS/MEADOWDIMENSION_Y);
    setBallMaterial(); // Postavljamo materijal na kugle
    for(i=0;i< NumOfSheeps;i++)
    {
       glPushMatrix();
       glTranslatef(Balls[i].pX,raiseBehindMeadow,Balls[i].pZ);
       glScalef(RADIUS/MEADOWDIMENSION_X,2,RADIUS/MEADOWDIMENSION_Z);
       glRotatef(Balls[i].angle,1,0,0);
       glutSolidSphere(1,20,20);
       glPopMatrix();
    }
}

void drawSheeps()
{
    for(int i=0;i< NumOfSheeps;i++)
    {
        glPushMatrix();
        glTranslatef(Balls[i].pX,1+ ((float)RADIUS/MEADOWDIMENSION_Y),Balls[i].pZ);
        glScalef(1/MEADOWDIMENSION_X,1/MEADOWDIMENSION_Y,1/MEADOWDIMENSION_Z);
        drawSheep(Balls[i].angle/180*PI);
        glPopMatrix();
    }
    */

    /* HACK:
    if(on_going)
    {
       glutTimerFunc(TIMER_LOWER_INTERVAL,jumping,TIMER_ID_JUMPING);
       glutTimerFunc(TIMER_LOWER_INTERVAL,jumping,TIMER_ID_JUMPING);
       glutTimerFunc(TIMER_LOWER_INTERVAL,jumping,TIMER_ID_JUMPING);
    }
     */



void drawObjects()
{
    int i;
    double raiseBehindMeadow = 1+ ((float)RADIUS/MEADOWDIMENSION_Y);
    for(i=0;i< NumOfSheeps;i++)
    {
        if(ifJustSheeps)
        {
            glPushMatrix();
            glTranslatef(Balls[i].pX,raiseBehindMeadow,Balls[i].pZ);
            glScalef(GlobalXSize,GlobalYSize,GlobalZSize);
            drawSheep(Balls[i].angle/180*PI);
            glPopMatrix();
        } else
        {
            setBallMaterial(); // Postavljamo materijal na kugle
            glPushMatrix();
            glTranslatef(Balls[i].pX,raiseBehindMeadow,Balls[i].pZ);
            glScalef(RADIUS * GlobalXSize,2, RADIUS * GlobalZSize);
            glRotatef(Balls[i].angle,1,0,0);
            glutSolidSphere(1,20,20);
            glPopMatrix();
        }
    }
}

void drawBlocks()
{
    if(minX == -1 && minZ == -1 &&
       maxX == 1  && maxZ ==  1)
        return;

    glTranslatef(0,1.5,0);

    glPushMatrix();
       glTranslatef(0,0,(1.0+maxZ) * 0.5);
       glScalef(2,1,1-maxZ);
       glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
       glTranslatef(0,0,(-1.0 + minZ) * 0.5);
       glScalef(2,1,minZ + 1);
       glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
       glTranslatef((1 + maxX)*0.5,0,0);
       glScalef((1 - maxX),1,2);
       glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
       glTranslatef((-1 + minX) * 0.5, 0, 0);
       glScalef(minX + 1,1,2);
       glutSolidCube(1);
    glPopMatrix();
}

/*
void drawClouds()
{
    glPointSize(1.5);
    glDisable(GL_LIGHTING);
    glColor3f(0.97,0.97,0.97);   // Boja oblaka
    srand(time(NULL)); //Inicijalizujemo random-generator koji ce nam nasumicno generisati pozicije tackica u oblacicicma
    float cX,cY,cZ;  // Koordinate tacke u oblaku
    float cXMin, cXMax, // Granicni opsezi u okviru kojih cemo crtati oblake
            cYMin, cYMax,
            cZMin, cZMax;
    int  i,thickness; // Redom, pomocna indeksna promenljiva i gustina oblaka(ukupan broj tackica u oblaku)
    cXMin = -5, cXMax = 5;  // Prvi crtamo tako da moze da se proteze od zadnjeg desnog coska
    cYMin = 4.5, cYMax = 6;
    cZMin = 0, cZMax = 10;
    glBegin(GL_POINTS);
    thickness=2000;
    for(i=0;i<thickness;i++)
    {
        cX= ((float)rand()/RAND_MAX)*(cXMax-cXMin)+ cXMin;
        cY= ((float)rand()/RAND_MAX)*(cYMax-cYMin)+ cYMin;
        cZ= ((float)rand()/RAND_MAX)*(cZMax-cZMin)+ cZMin;

        glVertex3f(cX,cY,cZ);

    }


    cXMin = -5, cXMax = 0;
    cZMin = -10, cZMax = 0;


    for(i=0;i<thickness;i++)
    {
        cX= ((float)rand()/RAND_MAX)*(cXMax-cXMin)+ cXMin;
        cY= ((float)rand()/RAND_MAX)*(cYMax-cYMin)+ cYMin;
        cZ= ((float)rand()/RAND_MAX)*(cZMax-cZMin)+ cZMin;
        //glBegin(GL_POINTS);
        glVertex3f(cX,cY,cZ);
        //glEnd();
    }


    cXMin = 0, cXMax = 5;
    cZMin = -10, cZMax = 10;


    for(i=0;i<thickness;i++)
    {
        cX= ((float)rand()/RAND_MAX)*(cXMax-cXMin)+ cXMin;
        cY= ((float)rand()/RAND_MAX)*(cYMax-cYMin)+ cYMin;
        cZ= ((float)rand()/RAND_MAX)*(cZMax-cZMin)+ cZMin;
        //glBegin(GL_POINTS);
        glVertex3f(cX,cY,cZ);
        //glEnd();
    }
    glEnd();
    glLineWidth(1);
    glEnable(GL_LIGHTING);
}
*/



void drawCloser()
{
    glPushMatrix();
    glTranslatef(Closer.pX,Closer.pY,Closer.pZ);
    glScalef(0.5*GlobalXSize,GlobalYSize,0.5*GlobalZSize);
    drawCylinder();

    glPopMatrix();
}


static void drawCylinder()
{
    float h,u;
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0,1,0);
    glVertex3f(0,1,0);
    for(u=0;u<=2*PI;u+= PI/20)
    {
        glVertex3f(cos(u),1,sin(u));
        glVertex3f(cos(u+PI/20),1,sin(u+PI/20));
    }
    glEnd();

    glBegin(GL_TRIANGLE_STRIP);
    for(h=1-PI/20;h>-1-EPSILON;h-=PI/20)
        for(u=0;u<=2*PI+ EPSILON;u+=PI/20)
        {
            glNormal3f(cos(u),0,sin(u));
            glVertex3f(cos(u),h,sin(u));
            glVertex3f(cos(u),h+PI/20,sin(u));
        }
    glEnd();
}
