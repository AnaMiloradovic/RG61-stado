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
#include "image.h"

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

GLuint textureNames;

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
    drawDecorativeGrass(1,0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(- 1 - 0.5 * 0.5*GlobalXSize,0,0); /*TODO: Da se iskomentarise*/
    glScalef(0.5 * GlobalXSize, GlobalYSize , 2 * (1 + 0.5*GlobalZSize) );
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    drawDecorativeGrass(-1,0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0,0,1 + 0.5 * 0.5*GlobalZSize); /*TODO: Da se iskomentarise*/
    glScalef(2 * (1 + 0.5 * GlobalXSize), GlobalYSize , 0.5 * GlobalZSize);
    glutSolidCube(1);
    glPopMatrix();
/*
    glPushMatrix();
    drawDecorativeGrass(1,1);
    glPopMatrix();*/
    glPushMatrix();
    glTranslatef(0,0, - 1 - 0.5 * 0.5 * GlobalZSize); /*TODO: Da se iskomentarise*/
    glScalef(2 * (1 + 0.5 * GlobalXSize),GlobalYSize, 0.5 * GlobalZSize);
    glutSolidCube(1);
    glPopMatrix();
    
    initializeTexture();
    applyTexture(-1,1,-1,1);
    /*glPushMatrix();
    drawDecorativeGrass(-1,1);
    glPopMatrix();*/

}

void drawHedge(float minR, float maxR, float fixed, int indX)
{
    setCloserMaterial();
    glPushMatrix();
    if(indX)
        glTranslatef((maxR + minR)/2.0,5,fixed);
    else
        glTranslatef(fixed,5,(maxR + minR)/2.0);

    glPushMatrix();
    if(indX)
    {
        glTranslatef(-(maxR-minR)*0.4,0,0);
        //glRotatef(1,0,0,1);
    }else {
        glTranslatef(0,0,-(maxR-minR)*0.4);
        glRotatef(2,0,1,0);
    }
    glScalef(0.3 * GlobalXSize, GlobalYSize,0.3 * GlobalZSize);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    if(indX)
    {
        glTranslatef((maxR-minR)*0.4,0,0);
        glRotatef(5,0,1,0);
    } else{
        glTranslatef(0,0,(maxR-minR)*0.4);
        glRotatef(5,0,1,0);
    }
    glScalef(0.3 * GlobalXSize,GlobalYSize,0.3 * GlobalZSize);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    if(indX)
    {
        glTranslatef(0,0.7,0);
        glRotatef(30,0,0,1);
        glScalef((maxR-minR),0.15 * GlobalYSize,0.15 * GlobalZSize);

    }
    else{
        glTranslatef(0,0.7,0);
        glRotatef(30,1,0,0);
        glScalef(0.15 * GlobalXSize,0.15 * GlobalYSize,(maxR-minR));
    }
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    if(indX)
    {
        glTranslatef(0,-0.7,0);
        glScalef((maxR-minR),0.15 * GlobalYSize,0.15 * GlobalZSize);
        glRotatef(-7,0,0,1);
    }else{
        glTranslatef(0,-0.7,0);
        glRotatef(-7,1,0,0);
        glScalef(0.15 * GlobalXSize,0.15 * GlobalYSize,(maxR-minR));
    }
    glutSolidCube(1);
    glPopMatrix();

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

void initializeTexture()
{
    Image * image;
    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV,
              GL_TEXTURE_ENV_MODE,
              GL_REPLACE);

    image = image_init(0, 0);

    char* fileName = "texture__grass.bmp";
    image_read(image, fileName);

    glGenTextures(1, &textureNames);

    glBindTexture(GL_TEXTURE_2D, textureNames);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);


    /* Iskljucujemo aktivnu teksturu */
    glBindTexture(GL_TEXTURE_2D, 0);

    /* Unistava se objekat za citanje tekstura iz fajla. */
    image_done(image);
}

void applyTexture(float minX, float maxX , float minZ , float maxZ)
{
    glBindTexture(GL_TEXTURE_2D, textureNames);
    glBegin(GL_QUADS);

    glTexCoord2f(0, 0);
    glVertex3f(minX, 1.1, minZ);

    glTexCoord2f(1, 0);
    glVertex3f(maxX, 1.1, minZ);

    glTexCoord2f(1, 1);
    glVertex3f(maxX, 1.1, maxZ);

    glTexCoord2f(0, 1);
    glVertex3f(minX, 1.1, maxZ);
    glEnd();


    /* Iskljucujemo aktivnu teksturu */
    glBindTexture(GL_TEXTURE_2D, 0);
}


void set_normal_and_vertex(float u, float v)
{
    glNormal3f(
            sin(v),
            0,
            cos(v)
    );
    glVertex3f(
            sin(v),
            u,
            cos(v)
    );
}



void setLightGrayMaterial()
{
    GLfloat materialCloserAmbient[] = {0.2,0.2,0.2,1};
    GLfloat materialCloserDiffuse[] = {0.8,0.8,0.8,1};
    GLfloat materialCloserSpecular[] = {0.9,0.9,0.9,1};
    GLfloat shininess=10;
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,materialCloserAmbient);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,materialCloserDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,materialCloserSpecular);
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS, shininess);
}






void drawObject()
{

    float u, v;

    glPushMatrix();
    glRotatef(10,0,0,1);
    glRotatef(-80,0,1,0);
    glScalef(0.5,0.5,0.5);
    glScalef(0.25,1.2,0.25);
    for (u = 0; u < PI; u += PI / 20) {
        glBegin(GL_TRIANGLE_STRIP);
        for (v = 0; v <= PI*2 + EPSILON; v += PI / 20) {   //NOTE: Dovoljna aproksimacija je 9 stepeni. ;)
            set_normal_and_vertex(u, v);
            set_normal_and_vertex(u + PI / 20, v);
        }
        glEnd();
    }
    glScalef(1.2,0.1,1.2);
    setLightGrayMaterial();
    for (u = 0; u < PI; u += PI / 20) {
        glBegin(GL_TRIANGLE_STRIP);
        for (v = 0; v <= PI*2 + EPSILON; v += PI / 20) {   //NOTE: Dovoljna aproksimacija je 9 stepeni. ;)
            set_normal_and_vertex(u, v);
            set_normal_and_vertex(u + PI / 20, v);
        }
        glEnd();
    }
    glScalef(1/(1.2*0.25),1/(0.1*1.2),1/(1.2*0.25));
    glRotatef(30,0,0,1);
    glutSolidCone(0.2,3,20,20);
    glTranslatef(0,0,3);
    glRotatef(60,0,0,1);
    //glutSolidCone(0.1,0.5,20,20);
    glPopMatrix();
    glEnd();
}





void drawCloser()
{
    glPushMatrix();
    glTranslatef(Closer.pX,Closer.pY,Closer.pZ);
    glScalef(0.5*GlobalXSize,GlobalYSize,0.5*GlobalZSize);
    drawObject();
    glPopMatrix();
}


/*
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
}*/

void drawDecorativeGrass(int translateParameter, int parameterZ)
{
    glScalef(1.0/MEADOWDIMENSION_X,1.0/MEADOWDIMENSION_Y,1.0/MEADOWDIMENSION_Z);
    if(parameterZ) glTranslatef(translateParameter* MEADOWDIMENSION_X,2*MEADOWDIMENSION_Y,-MEADOWDIMENSION_Z + 0.5);
    else glTranslatef(-MEADOWDIMENSION_X+0.5,2*MEADOWDIMENSION_Y,translateParameter*MEADOWDIMENSION_Z);
    GLdouble equation[] = {0,1,0,0};
    glClipPlane(GL_CLIP_PLANE0,equation);
    glEnable(GL_CLIP_PLANE0);
    GLdouble equation2[] = {0,0,-1,MEADOWDIMENSION_Z + 1};
    if(parameterZ)
    {
        equation2[0] = -1;
        equation2[2] = 0;
        equation2[3] = MEADOWDIMENSION_X + 1;
    }
    glClipPlane(GL_CLIP_PLANE1,equation2);
    glEnable(GL_CLIP_PLANE1);
    int i,i_max = 8*2;
    if(parameterZ) i_max *= 2;
    for(i=0;i<i_max;i++) {
        glPushMatrix();
        if(parameterZ)
            glScalef(1,1,MEADOWDIMENSION_Z/(0.8*0.2));
        else glScalef( MEADOWDIMENSION_X / 8.0, 1, 1);
        if(parameterZ)
            glRotatef(-45,1,0,0);
        else glRotatef(-45, 0, 0, 1);
        glutSolidCube(1);
        glPopMatrix();
        if(parameterZ)
            glTranslatef(0,0,MEADOWDIMENSION_X/8.0);
        else glTranslatef(MEADOWDIMENSION_X/8.0,0,0);
    }
    glDisable(GL_CLIP_PLANE0);
    glDisable(GL_CLIP_PLANE1);
}
