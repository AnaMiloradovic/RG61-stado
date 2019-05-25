/* Izvorna datoteka sa implementacijama funkcija koje iscrtavaju objekte u igri i funkcijama za teksture. */
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "inits.h" /* Neophodno zbog deklaracije strukture zatvarajuceg objekta i strukture ovaca. */
#include "draw_objects.h"
#include "light_and_materials.h" /* Neophodno zbog funkcija koje postavljaju materijale na nase objekte. */
#include "sheep.h" /* Ovde nam je deklarisana funkcija za iscrtavanje ovcice. */
#include "image.h" /* Biblioteka asistenata sa kursa "Racunarska grafika" - za upotrebu tekstura. */

/* Deklaracije neophodnih promenljivih iz drugih datoteka. */
extern BALL Balls[];  /* Niz struktura ovaca - samo naziv ostao, jer je igra na pocetku razvijana sa kuglama. */
extern CLOSER Closer; /* Promenljiva - struktura zatvarajuceg objekta. (kose) */
extern int NumOfSheeps;  /* Broj ovaca u igri. */
extern float minX,maxX,minZ,maxZ; /* Unutar ovih granica X i Z koordinata je slobodna povrsina za ovce. */

/*--- Jedinice u svetskom koordinatnom sistemu. */
extern double GlobalXSize;    /* Zapravo, vrednosti ove 3 promenljive su 1/MEADOW_DIMENSION_(X|Y|Z). Posto su nam dok iscrtavamo po terenu skoro */
extern double GlobalYSize;    /* sve dimenzije izrazene u odnosu na dimenzije terena (tj. nisu dimenzije svetskog koordinatnog sistema), mnozeci ih sa */
extern double GlobalZSize;    /* ovim promenljivama mi zapravo vrsimo implicitnu inverznu transformaciju na veoma citljiv nacin koji moze lepo da se tumaci. */
/*---   */

GLuint textureNames; /* Promenljiva u kojoj cuvamo generisano 'ime' teksture koju koristimo. */

void drawMeadow() /* Funkcija za iscrtavanje livade - terena za igru. */
{
    glScalef(MEADOWDIMENSION_X, MEADOWDIMENSION_Y, MEADOWDIMENSION_Z); /* Od ovog skaliranja su sve dalje dimenzije izrazene u odnosu na njih.
      Pojam dimenzije terena nam je zapravo polovina dimenzije (polovina duzine ili sirine), da bi smo posle sve koordinate polozaja posmatrali u opsegu [-1,1]. */
    glutSolidCube(2);          /* Ako hocemo da koristimo svetske koordinate, te vrednosti mnozimo sa GLobal*Size promenljivama. */

    /* Sa prethodnim glutSolidCube pozivom smo iscrtali sam teren za ovcice. Sada treba da iscrtamo njegove ivice. */

    glPushMatrix();
       glTranslatef(1 + 0.5 * 0.5*GlobalXSize,0,0); /* Transliramo na ivicu terena (ovde na desnu duzu ivicu terena). Translacija za citavu polovinu duzine + pola sirine ivice koju cemo nacrtati.(koja ce biti bas 0.5, zbog cega koristimo GlobalXSize).*/
       glScalef(0.5 * GlobalXSize,1 * GlobalYSize, 2 * (1 + 0.5*GlobalZSize) ); /* I za samu dimenziju ivica ce nam biti lakse da navedemo stvarne dimenzije kakve hocemo. */
       glutSolidCube(1); /* Posto smo skalirali sve dimenzije kako nam odgovaraju, crtamo transformisanu 'kocku' (zapravo kvadar) koji ce imati zeljene dimenzije. */
    glPopMatrix();   /* Isti je princip i za ostale tri ivice. */

    glPushMatrix();
       glTranslatef(- 1 - 0.5 * 0.5*GlobalXSize,0,0); 
       glScalef(0.5 * GlobalXSize, GlobalYSize , 2 * (1 + 0.5*GlobalZSize) );
       glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
       glTranslatef(0,0,1 + 0.5 * 0.5*GlobalZSize);
       glScalef(2 * (1 + 0.5 * GlobalXSize), GlobalYSize , 0.5 * GlobalZSize);
       glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
       glTranslatef(0,0, - 1 - 0.5 * 0.5 * GlobalZSize);
       glScalef(2 * (1 + 0.5 * GlobalXSize),GlobalYSize, 0.5 * GlobalZSize);
       glutSolidCube(1);
    glPopMatrix();
    
    /* Iscrtavanje dekorativne trave duz kracih ivica terena. Bice pojasnjeno kod definicije odgovarajuce funkcije. */
    glPushMatrix();
       drawDecorativeGrass(1,0);
    glPopMatrix();

    glPushMatrix();
       drawDecorativeGrass(-1,0);
    glPopMatrix();
         
    /* Inicijalizacija i primena teksture na povrsinu gde ovcice trce. */
    initializeTexture();
    applyTexture(-1,1,-1,1);
}

void drawHedge(float minR, float maxR, float fixed, int indX) /* Funkcija za iscrtavanje ogradice oko ovcica na kraju partije. */
{                                                            /* Namerno su grede pomalo iskrivljene, da bi izgledalo vernije. */
    setCloserMaterial(); /* Isti materijal se koristi kao i za drsku kose. */

    glPushMatrix();  /* Transliramo koordinatni sistem u drugi koordinatni pocetak u odnosu na koji cemo da crtamo. */
       if(indX)
           glTranslatef((maxR + minR)/2.0,5,fixed);
       else
           glTranslatef(fixed,5,(maxR + minR)/2.0);

       glPushMatrix(); 
          if(indX)       /* Leva (ili gornja ako crtamo duz z-ose) uspravna greda. */
          {
           glTranslatef(-(maxR-minR)*0.4,0,0);
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
            glPushMatrix();
            glTranslatef(Balls[i].pX,raiseBehindMeadow,Balls[i].pZ);
            glScalef(GlobalXSize,GlobalYSize,GlobalZSize);
            drawSheep(Balls[i].angle/180*PI);
            glPopMatrix();
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
