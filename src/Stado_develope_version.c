/* Glavni fajl aplikacije. U okviru njega se nalaze main i on_display funkcija. */

#include <GL/glut.h>
#include <stdio.h>
#include <time.h>

#include "colors.h" /* Potrebno, jer su u njemu simbolicke konstante za boju neba danju i nocu. */
#include "inits.h" /* Ovde su nam deklarisane inicijalizacione funkcije. */
#include "draw_objects.h" /* Ovde su nam deklarisane funkcije za iscrtavanje objekata u igri. */
#include "light_and_materials.h" /* Ovde su nam deklarisane funkcije za podesavanje osvetljenja i postavljanje materijala. */
#include "game_dynamics.h" /* Ovde su nam deklarisane funkcije za dinamiku objekata u igrici. */

const char pause[] = "Pauza"; /* String od kog pravimo bitmapu za pauzu i duzina tog stringa. */
#define PAUZE_LEN (5)

/* Deklaracije neophodnih promenljivih iz drugih datoteka. */
extern CLOSER Closer; /* Promenljiva - struktura zatvarajuceg objekta. (kose) */
extern float minX,maxX,minZ,maxZ; /* Unutar ovih granica X i Z koordinata je slobodna povrsina za ovce. */
extern int on_going; /* Glavna kontrolna promenljiva igre - da li je igra u toku ili moze da se tumaci kao da li se objketi krecu, animacija aktivna. */
extern int winner;   /* Kontrolna promenljiva igre - da li je igrac pobedio. */
extern int hit;      /* Jos jedna vazna kontrolna promenljiva igre - da li igrac trenutno iscrtava zatvarajucu putanju. */
extern int timePast; /* Koliko je do sad puta pozvan tajmer - koliko vremena je proteklo */
extern GLuint textureNames; /* 'Ime' teksture koju smo koristili za teren. */

extern struct tm* today; /* Struktura struct tm koja cuva tekuci dan. */

float to_dark_R = (((float) COLOR_SKY_R - DARK_BLUE_R) / TIME_OUT); /* Korak za gasenje svetla i prelazak iz svetlo plave boje u tamno plavu. */
float to_dark_G = (((float) COLOR_SKY_G - DARK_BLUE_G) / TIME_OUT); /* Imamo ukupno TIME_OUT poziva za prelazak, i u svakom od poziva podjednako smanjujemo R, G i B komponentu boje i svetla. */
float to_dark_B = (((float) COLOR_SKY_B - DARK_BLUE_B) / TIME_OUT);

float to_dark_R_S = (((float) COLOR_SKY_R - DARK_BLUE_R) / (TIME_OUT * 1.5)); /* Korak za gasenje svetla po letnjem racunanju vremena i prelazak iz svetlo plave boju u tamno plavu. */
float to_dark_G_S = (((float) COLOR_SKY_G - DARK_BLUE_G) / (TIME_OUT * 1.5)); /* (Dan traje duze) */
float to_dark_B_S = (((float) COLOR_SKY_B - DARK_BLUE_B) / (TIME_OUT * 1.5));

int main(int argc, char* argv[])
{
   gameDataInitialization(); /* Inicijalizacija osnovnih podataka o igri. */
   glutInit(&argc,argv);  /* Neophodna inicijalizacija GLUT-a. */
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
   
   graphicsInitialization(); /* Graficka inicijalizacija. */
   glutMainLoop();           /* Program ulazi u glavnu petlju. */
   return 0;
}

void onDisplayFunction()
{
   /* Postavljamo boju neba u skladu sa proteklim vremenom i ostale OpenGL inicijalizacije. */
    
    if(today != NULL && today->tm_mon >= 5 && today->tm_mon <= 8)
    {
       glClearColor(COLOR_SKY_R -  to_dark_R_S * timePast, 
                    COLOR_SKY_G -  to_dark_G_S * timePast,
                    COLOR_SKY_B -  to_dark_B_S * timePast, 0);
    }

    else {
        glClearColor(COLOR_SKY_R -  to_dark_R * timePast, 
                     COLOR_SKY_G -  to_dark_G * timePast,
                     COLOR_SKY_B -  to_dark_B * timePast, 0);      
    }
    glClearColor(COLOR_SKY_R -  to_dark_R * timePast, 
                 COLOR_SKY_G -  to_dark_G * timePast,
                 COLOR_SKY_B -  to_dark_B * timePast, 0);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_LIGHTING); 
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);

    setSunLight(); /* Postavljamo dnevno osvetljenje. */


    glMatrixMode(GL_MODELVIEW);  /* Podesavamo polozaj kamere. */
    glLoadIdentity();
    if(winner)                                         /* Ako je igrac pobedio, priblizicemo kameru blize zatvorenim ovcicama. */
        gluLookAt(MEADOWDIMENSION_X * (maxX + minX) * 0.5, 10.0 * MEADOWDIMENSION_Y, MEADOWDIMENSION_Z * (maxZ + 0.15),
                MEADOWDIMENSION_X * (maxX + minX) * 0.5, 3.0 * MEADOWDIMENSION_Y, MEADOWDIMENSION_Z * (maxZ + minZ) * 0.5,
                  0,1,0);

    else if(Closer.pZ*MEADOWDIMENSION_Z <= 0)          /* Ako kosa ode mnogo daleko po z-osi, pocinjemo i mi da idemo za njom. */
        gluLookAt(0,5,Closer.pZ*MEADOWDIMENSION_Z+13,Closer.pX*MEADOWDIMENSION_X/10.0,Closer.pY*MEADOWDIMENSION_Y+1,Closer.pZ*MEADOWDIMENSION_Z,0,1,0);

    else                                               /* Podrazumevani polozaj kamere. */
        gluLookAt(0,5,13,Closer.pX*MEADOWDIMENSION_X/10.0,Closer.pY*MEADOWDIMENSION_Y+1,Closer.pZ*MEADOWDIMENSION_Z,0,1,0); // Pogled(posmatraceva pozicija, tacka pogleda, 'vektor nagore')


    setMeadowMaterial(); /* Postavljamo materijal na livadu. */
    glPushMatrix();  /* Iscrtavamo livadu(glavni teren) gde ce nam trcati ovce. */
       drawMeadow(); /* Ne radimo glPopMatrix(), jer hocemo da nam na dalje koordinatni sistem bude vezan za teren. */
       glPushMatrix();   /* Iscrtavamo zatvorene povrsine. */
          drawBlocks();
       glPopMatrix();

       initialPos();   /* Inicijalizujemo pocetne pozicije ovaca, zatvarajuceg objekta i pocetne vrednosti za minX, maxX, minZ i maxZ. 
                          Funkcija ce se izvrsavati zapravo samo pri prvom pozivu display zbog provere indikatora unutar funkcije na pocetku. */
       glPushMatrix();
          drawObjects();  /* Crtamo ovce. */
       glPopMatrix();


       glPushMatrix();
          setCloserMaterial();
          if(!winner)
            drawCloser();  /* Iscrtavamo kosu. */
       glPopMatrix();

       if(hit)      /* Ako trenutno iscrtavamo zatvarajucu putanju, pozivamo odgovarajuce funkcije. */
       {
         drawHitting();
         drawHittingPath();
       }

       setMeadowMaterial(); 
       glPushMatrix();
          drawBlocks();  /* Crtamo zagradjene povrsine. */
       glPopMatrix();

       if(winner)  /* Ako je igrac vec pobedio, crtamo ogradu oko zagradjenih ovaca. */
       {
          glPushMatrix(); /* Desno po duzini. */
             drawHedge(minZ,maxZ,maxX,0);
          glPopMatrix();

          glPushMatrix(); /* Levo po duzini. */
             drawHedge(minZ,maxZ,minX,0);
          glPopMatrix();

          glPushMatrix(); /* Nazad po sirini. */
             drawHedge(minX,maxX,minZ,1);
          glPopMatrix();

          glPushMatrix(); /* Napred po sirini. */
             drawHedge(minX,maxX,maxZ,1);
          glPopMatrix();
       }
    glPopMatrix(); /* Od sada vise nismo u koordinatnom sistemu vezanom za teren. */

    /* Proveravamo da nije vec isteklo vreme - dovoljan broj poziva tajmera. */
    if((today != NULL && today->tm_mon >= 5 && today->tm_mon <= 8 && timePast == 1.5 * TIME_OUT)   ||
    ((  today == NULL || today->tm_mon <  5 || today->tm_mon > 8 ) && timePast == TIME_OUT)  )
    {
        printf("Isteklo vreme - kraj igre \n");
        glDeleteTextures(1,&textureNames);
        exit(EXIT_SUCCESS);
    }

    else if(timePast == NIGHT_NEAR) /* Ako je kraj igre vrlo blizu, upozoravamo igraca. */
        printf("Pozuri! Noc samo sto nije skroz pala!\n");

    if(on_going) /* Ako je jos uvek igra u toku (nije isteklo vreme, trenutno nije pauza i igrac jos nije pobedio), registruje se tajmer. */
    {
       glutTimerFunc(TIMER_INTERVAL,rollingBalls,TIMER_ID_ROLLING);
       timePast++;
    }

    else if(!winner) /* Ako nije igra u toku, a igrac nije pobedio, pauza je. */
    {
        glPushMatrix();
        setLetters();
        glRasterPos3f(-0.5,1.5,1.5);
        for(int i=0;i<PAUZE_LEN;i++)
           glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,pause[i]);
    }
    
    glutSwapBuffers(); /* Slika se salje na ekran. */
}
