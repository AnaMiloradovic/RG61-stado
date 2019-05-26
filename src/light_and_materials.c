/* Ovo je izvorna datoteka sa implementacijama funkcija za postavljanje materijala i za postavljanje
   dnevnog svetla na scenu. */
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "light_and_materials.h"

extern int timePast; /* Koliko je do sad puta pozvan tajmer - koliko vremena je proteklo */
extern float to_dark_R; /* Koraci za gasenje svetla, po svakoj komponenti boje. Izracunate su u fajlu Stado_develope_version.c i cuvamo ih    */
extern float to_dark_G; /* bas kao promenljive - konstante da se ne bi vise puta racunale i da ne bismo morali nijedno drugo zaglavlje ovde da ukljucujemo. */
extern float to_dark_B;

extern float to_dark_R_S; /* Koraci za gasenje svetla po letnjem racunanju vremena. */
extern float to_dark_G_S;
extern float to_dark_B_S;

extern struct tm* today;

void setSunLight() /* Funkcija za postavljanje dnevnog svetla na sceni. */
{
    GLfloat light_position[] = { 0, 100, 100, 0 };  /* Pozicija svetla - posto je Suncevo svetlo u pitanju, stavicemo da je direkciono svetlo. */
    GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1 }; /* Ambijentalna komponenta svetla. */

    /* Difuzna komponenta svetla. */
    /* Tokom igre postepeno pada noc, pa se i svetlo postepeno 'gasi'. */
    GLfloat light_diffuse[] = { 0.7 - to_dark_R * timePast,
                                0.7 - to_dark_G * timePast,
                                0.7 - to_dark_B * timePast, 1 };
    
    /* Ako je leto, partija traje duze. */
    if(today != NULL && today->tm_mon >= 5 && today->tm_mon <= 8)
    {
        light_diffuse[0] = 0.7 + to_dark_R * timePast - to_dark_R_S * timePast;
        light_diffuse[1] = 0.7 + to_dark_G * timePast - to_dark_G_S * timePast;
        light_diffuse[2] = 0.7 + to_dark_B * timePast - to_dark_B_S * timePast;
    }

    /* Spekularna boja svetla, i nju menjamo u skladu sa proteklim vremenom igre. */
    GLfloat light_specular[] = { 0.9 - to_dark_R * timePast,
                                 0.9 - to_dark_G * timePast,
                                 0.9 - to_dark_B * timePast, 1 };

    /* Ako je leto, parija traje duze. */
    if(today != NULL && today->tm_mon >= 5 && today->tm_mon <= 8)
    {
        light_specular[0] = 0.9 + to_dark_R * timePast - to_dark_R_S * timePast;
        light_specular[1] = 0.9 + to_dark_G * timePast - to_dark_G_S * timePast;
        light_specular[2] = 0.9 + to_dark_B * timePast - to_dark_B_S * timePast;
    }



    glLightfv(GL_LIGHT0, GL_POSITION, light_position); /* Registrujemo zadate parametre dnevnog svetla. */
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
}

void setMeadowMaterial() /* Funkcija koja postavlja travnati materijal na livadu. */
{
    GLfloat materialGrassAmbient[]= {0.2,0.6,0.2,1}; /* Zadajemo ambijentalnu i difuznu komponentu materijala. */
    GLfloat materialGrassDiffuse[]= {0.05,1,0.05,1};
    
    /* Postavljanje materijala na livadu. */
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,materialGrassAmbient);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,materialGrassDiffuse);

}

void setCloserMaterial() /* Funkcija koja postavlja materijal na drsku kose. (gornji, drveni deo kose) */
{
    /* Zadajemo zeljene vrednosti parametara. */
    GLfloat materialCloserAmbient[] = {0.3,0.1,0.1,1};
    GLfloat materialCloserDiffuse[] = {0.4,0.1,0.1,1};
    GLfloat materialCloserSpecular[] = {0.5,0.5,0.5,1};
    GLfloat shininess=10;

    /* Postavljamo drveni materijal. */
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,materialCloserAmbient);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,materialCloserDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,materialCloserSpecular);
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS, shininess);
}

void setLightGrayMaterial() /* Funkcija koja postavlja materijal na dug i ostar metalni donji deo kose. */
{
    /* Zadajemo zeljene vrednosti parametara. */
    GLfloat materialCloserAmbient[] = {0.2,0.2,0.2,1};
    GLfloat materialCloserDiffuse[] = {0.8,0.8,0.8,1};
    GLfloat materialCloserSpecular[] = {0.9,0.9,0.9,1};
    GLfloat shininess=10;

    /* Postavljamo metalni materijal. */
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,materialCloserAmbient);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,materialCloserDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,materialCloserSpecular);
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS, shininess);
}

void setLetters() /* Postavljamo materijal na slova bitmape koja se prikazuje u vreme pauze. */
{
    GLfloat letters[] = {1,1,1,1}; /* Zeljene vrednosti parametara. */

    /* Postavljamo materijal. */
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,letters);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,letters);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,letters);
}