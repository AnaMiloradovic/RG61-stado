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
/* HACK: extern SURFACE ClosedSurfaces[MAX_POSSIBLE_CLOSED_SURFACES];  */
/* HACK: extern int NumOfClosedSurfaces;   */
extern int NumOfSheeps, Level;
extern int on_going;

void drawMeadow()
{
    glScalef(MEADOWDIMENSION_X, MEADOWDIMENSION_Y, MEADOWDIMENSION_Z);
    glutSolidCube(2);

    // Iscrtavamo ivice duz terena (i duz z-ose)
    glPushMatrix();
    glScalef(0.5/MEADOWDIMENSION_X,1/MEADOWDIMENSION_Y,(1+0.5/MEADOWDIMENSION_Z)*2);
    //Crtamo sada svaku ivicu ponaosob od njih
    glPushMatrix();
    glTranslatef(MEADOWDIMENSION_X/0.5+0.5,0,0);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-(MEADOWDIMENSION_X/0.5 + 0.5),0,0);
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();
    // Ivice po sirini terena (formalnije objasnjeno, duz x-ose)
    glPushMatrix();
    glScalef(2*(1 + 0.5/MEADOWDIMENSION_X),4,0.5/10);
    //Crtamo sada svaku ivicu ponaosob od njih
    glPushMatrix();
    glTranslatef(0,0,MEADOWDIMENSION_Z/0.5+0.5);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0,0,-(MEADOWDIMENSION_Z/0.5 + 0.5));
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();
}

void drawBalls()
{
    int i;
    setBallMaterial(); // Postavljamo materijal na kugle
    for(i=0;i< NumOfSheeps;i++)
    {
       glPushMatrix();
       glTranslatef(Balls[i].pX,1+ ((float)RADIUS/MEADOWDIMENSION_Y),Balls[i].pZ);
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

    /* HACK:
    if(on_going)
    {
       glutTimerFunc(TIMER_LOWER_INTERVAL,jumping,TIMER_ID_JUMPING);
       glutTimerFunc(TIMER_LOWER_INTERVAL,jumping,TIMER_ID_JUMPING);
       glutTimerFunc(TIMER_LOWER_INTERVAL,jumping,TIMER_ID_JUMPING);
    }
     */
}

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

void drawCloser()
{
    glPushMatrix();
    glTranslatef(Closer.pX,Closer.pY,Closer.pZ);
    glScalef(0.5*((float) 1/MEADOWDIMENSION_X),(float)1/MEADOWDIMENSION_Y,0.5*((float) 1/MEADOWDIMENSION_Z));
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
    glPopMatrix();
}

/* HACK:

void drawClosedSurfaces()
{
    setMeadowMaterial();
    for(int i=0;i<NumOfClosedSurfaces;i++) {
        glPushMatrix();
           glTranslatef((ClosedSurfaces[i].maxX + ClosedSurfaces[i].minX)/2,1,(ClosedSurfaces[i].maxZ + ClosedSurfaces[i].minZ)/2);
           glScalef(ClosedSurfaces[i].maxX - ClosedSurfaces[i].minX,0.75*(1.0/MEADOWDIMENSION_Y),ClosedSurfaces[i].maxZ - ClosedSurfaces[i].minZ);
           glutSolidCube(1);
        glPopMatrix();
    }
}




void jumping(int timer_id)
{
    if(timer_id != TIMER_ID_JUMPING)
        return;
    for(int i=0;i<NumOfSheeps;i++)
    {
        Balls[i].angle += Balls[i].w_angle;
    if(Balls[i].angle > 360)
        Balls[i].angle -= 360;
    else if(Balls[i].angle < 360)
        Balls[i].angle += 360;
    }
    glutPostRedisplay();
}
    */