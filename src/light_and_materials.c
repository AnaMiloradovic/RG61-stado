#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "colors.h" //Ovde cuvamo boje koje koristimo u igri
#include "inits.h" //Ovde nam se nalaze deklaracije inicijalizacionih funkcija
#include "callbacks.h" //Callback funckije
#include "draw_objects.h" //Funckije za iscrtavanje objekata
#include "light_and_materials.h" //Funkcije za podesavanje osvetljenja i postavljanje materijala
extern int timePast;
void setSunLight()
{
    GLfloat light_position[] = { 0, 100, 100, 0 };
    GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1 };

    /* Difuzna boja svetla. */
    GLfloat light_diffuse[] = { 0.7 - (((float) COLOR_SKY_R - DARK_BLUE_R) / TIME_OUT) * timePast,
                                0.7 - (((float) COLOR_SKY_R - DARK_BLUE_G) / TIME_OUT) * timePast,
                                0.7 - (((float) COLOR_SKY_R - DARK_BLUE_B) / TIME_OUT) * timePast, 1 };

    /* Spekularna boja svetla. */
    GLfloat light_specular[] = { 0.9 - (((float) COLOR_SKY_R - DARK_BLUE_R) / TIME_OUT) * timePast,
                                 0.9 - (((float) COLOR_SKY_R - DARK_BLUE_G) / TIME_OUT) * timePast,
                                 0.9 - (((float) COLOR_SKY_R - DARK_BLUE_B) / TIME_OUT) * timePast, 1 };
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
}

void setMeadowMaterial()
{
    // Opis materijala od kog je napravljena livada
    GLfloat materialGrassAmbient[]= {0.2,0.6,0.2,1};
    GLfloat materialGrassDiffuse[]= {0.05,1,0.05,1};
    GLfloat materialGrassSpecular[]= {0,1,0,1};
    GLfloat shininess=30;
    
    // Postavljanje materijala na livadu
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,materialGrassAmbient);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,materialGrassDiffuse);
    //glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,materialGrassSpecular);
    //glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS, shininess);
}

void setBallMaterial()
{
    //Podesavamo odgovarajuce materijale za kugle
    GLfloat materialBallAmbient[] = {0.75,0.75,0.75,1};
    GLfloat materialBallDiffuse[] = {0.8,0.8,0.8,1};
    GLfloat materialBallSpecular[] = {0.75,0.8,0.75,1};
    GLfloat shininess=30;
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,materialBallAmbient);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,materialBallDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,materialBallSpecular);
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS, shininess);
}

void setCloserMaterial()
{
    //Podesvamo odgovarajuci materijal za valjak
    GLfloat materialCloserAmbient[] = {0.3,0.1,0.1,1};
    GLfloat materialCloserDiffuse[] = {0.4,0.1,0.1,1};
    GLfloat materialCloserSpecular[] = {0.5,0.5,0.5,1};
    GLfloat shininess=10;
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,materialCloserAmbient);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,materialCloserDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,materialCloserSpecular);
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS, shininess);
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

void setLetters()
{
    GLfloat letters[] = {1,1,1,1};
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,letters);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,letters);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,letters);
}
/*TODO
void setStarMaterial()
{
    GLfloat ambient_coeffs[] = { 0.8, 0.8, 0.8, 1 };
    GLfloat diffuse_coeffs[] = { .7, .7, .7, 1 };
    GLfloat emission_coeffs[] = {1,1,220.0/256,0.25};

    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT, GL_EMISSION, emission_coeffs);
}*/
