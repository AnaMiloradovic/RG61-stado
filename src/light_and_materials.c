#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "colors.h" //Ovde cuvamo boje koje koristimo u igri
#include "inits.h" //Ovde nam se nalaze deklaracije inicijalizacionih funkcija
#include "callbacks.h" //Callback funckije
#include "draw_objects.h" //Funckije za iscrtavanje objekata
#include "light_and_materials.h" //Funkcije za podesavanje osvetljenja i postavljanje materijala



void setSunLight()
{   
    // Pozicija i komponente osvetljenja
    GLfloat lightPosition[] = {-1,10,-1,0};
    GLfloat lightAmbient[]  = {1,1,1,1};
    GLfloat lightDiffuse[]  = {0.7,0.7,0.7,1};
    GLfloat lightSpecular[] = {1,1,1,1};
    
    // Podesavanje osvetljenja
    glLightfv(GL_LIGHT0,GL_POSITION,lightPosition);
    glLightfv(GL_LIGHT0,GL_AMBIENT,lightAmbient);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,lightDiffuse);
    glLightfv(GL_LIGHT0,GL_SPECULAR,lightSpecular);
}

void setMeadowMaterial()
{
    // Opis materijala od kog je napravljena livada
    GLfloat materialGrassAmbient[]= {0,0.3,0,1};
    GLfloat materialGrassDiffuse[]= {0,0.4,0,1};
    GLfloat materialGrassSpecular[]= {0,1,0,1};
    GLfloat shininess=30;
    
    // Postavljanje materijala na livadu
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,materialGrassAmbient);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,materialGrassDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,materialGrassSpecular);
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS, shininess);
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
    GLfloat materialCloserSpecular[] = {0.95,0.5,0.5,1};
    GLfloat shininess=30;
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,materialCloserAmbient);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,materialCloserDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,materialCloserSpecular);
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS, shininess);
}