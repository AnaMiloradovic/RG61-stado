/* Izvorna datoteka sa implementacijama callback funkcija koje koristimo u programu. */
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "inits.h"
#include "callbacks.h" /* Neophodno zbog deklaracije strukture za zatvarajuci objekat. */

/* Sirina i visina prozora aplikacije. */
int width,height;

/* Deklaracije neophodnih promenljivih iz drugih datoteka. */
extern int on_going;    /* Indikator da li je igra u toku, tj. da li se krecu objekti u igri. */
extern CLOSER Closer;   /* Promenljiva - struktura koja opisuje kosu, zatvarajuci objekat. */
extern int hit;         /* Indikator da li je u toku iscrtavanje zatvarajuce putanje. */
extern char curDir;     /* Promenljiva koja cuva tekuci pravac i smer kretanja kose u toku iscrtavanja
                           zatvarajuce putanje - neophodno za detekciju skretanja. */
extern float maxX, minX, maxZ, minZ;  /* Unutar ovih granica X i Z koordinata je slobodna povrsina za ovce. */
extern GLuint textureName;  /* U okviru ove promenljive je 'ime' teksture koju koristimo. */

void onReshapeFunction(int w, int h) /* Svaki put kada ucitamo prozor ili mu promenimo velicinu se poziva ova funkcija. */
{                                    /* Ovde podesavamo viewport - kako da nam se mapira i podesavamo projekciju. */
    width=w;
    height=h;
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(90,(float) width/height,1,40); 
}

void onKeyboardFunction(unsigned char key, int x, int y) /* Odgovarajuce reakcije na stisak 'klasicnih' tastera na tastaturi. */
{
    switch(key)
    {
        case 27:  /* esc - taster, Ako se pobedi u igri, treba ga stisnuti da se izadje iz igre. */
            printf("Izasli ste iz igre.");  /* Ako se klikne u sred igre, momentalno se ista napusta, bez obzira na stanje igre. */
            glDeleteTextures(1, &textureName);  /* Oslobadjamo resurs od teksture. */
            exit(0);
            break;
        case 'p':  /* Za pauzu. Pauza ne utice na stanje igre, i ponovnim kliktanjem istog tastera igrac normalno nastavlja gde je stao. */
            on_going = !on_going;
            if(on_going)
                glutPostRedisplay();
            break;
        case ' ': /* Ovime nagovestavamo da hocemo da pocnemo da iscrtavamo zatvarajucu putanju.*/
            if(!hit)  /* Jednom kada se klikne, posle ga nije potrebno ni stiskati, ni drzati do kraja tekuce putanje.(stavljena i provera da zapravo vec iscrtavanje nije registrovano) */
                hit = 1; /* Da bi iscrtavanje uopste krenulo neophodno je da kosa bude na nekoj od ivica slobodne povrsine(ta provera je drugde u kodu).*/
            break;
    }
}

void onKeyboardSpecialFunc(int key, int x, int y) { /* Odgovarajuce reakcije na stisak tzv. 'specijalnih' tastera. */
    glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);          /* Medjutim, mi ovde imamo samo reakcije na strelice, i one nam sluze za pokretanje kose za vreme i van isctavanja zatvarajuce putanje. */
    switch (key) {
        case GLUT_KEY_UP:
            if(Closer.pZ - Closer.vZ >= minZ - 0.1)
               Closer.pZ -= Closer.vZ;   /* Za odgovarajuci broj se menja odgovarajuca (ovde z) koordinata, povecava ili, poput ovde, smanjuje. */
            if (Closer.pZ < minZ)  /* Ne damo kosi da ide van granica slobodne povrsine terena. Osim sto ovim ne dozvoljavamo kosi da 'iskoci' daleko od terena za igru, */
                Closer.pZ = minZ;  /* bez njega bi nam bilo gotovo nemoguce da tempiramo bas ivicu kada hocemo da krenemo u iscrtavanje. (onako bismo morali da 'nabodemo' ivicu terena) */
            if(hit && curDir != 'u') /* Ovim, ako je iscrtavanje u toku, osvezavamo trenutni pravac i smer kretanja kose. */
               curDir = 'u';
            break;   /*       ---- Slican princip je i sa svim ostalim strelicama i pravcima/smerovima kretanja. */
        case GLUT_KEY_DOWN:
            if(Closer.pZ + Closer.vZ <= maxZ + 0.1)
               Closer.pZ += Closer.vZ;
            if (Closer.pZ > maxZ)
                Closer.pZ = maxZ;
            if(hit && curDir != 'd')
                curDir = 'd';
            break;
        case GLUT_KEY_LEFT:
            if(Closer.pX - Closer.vX >= minX - 0.1)
                Closer.pX -= Closer.vX;
            if (Closer.pX < minX)
                Closer.pX = minX;
            if(hit && curDir!= 'l')
                curDir = 'l';
             break;
        case GLUT_KEY_RIGHT:
             if(Closer.pX + Closer.vX <= maxX + 0.1)
                Closer.pX += Closer.vX;
             if (Closer.pX > maxX)
                Closer.pX = maxX;
             if(hit && curDir != 'r')
                 curDir = 'r';
            break;
    }

}
