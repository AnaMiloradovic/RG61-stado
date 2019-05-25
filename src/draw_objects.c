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
/*---  ---------------------------------------- */



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
       drawDecorativeGrass(1);
    glPopMatrix();

    glPushMatrix();
       drawDecorativeGrass(-1);
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
          if(indX)              /* Leva (ili zadnja ako crtamo duz z-ose) uspravna greda. */
          {
              glTranslatef(-(maxR-minR)*0.4,0,0);
    }     else {
              glTranslatef(0,0,-(maxR-minR)*0.4);
              glRotatef(2,0,1,0);
    }
          glScalef(0.3 * GlobalXSize, GlobalYSize,0.3 * GlobalZSize);
          glutSolidCube(1);
       glPopMatrix();

       glPushMatrix();
           if(indX)                    /* Desna (prednja) uspravna greda. */
           {
              glTranslatef((maxR-minR)*0.4,0,0);
              glRotatef(5,0,1,0);
    }     else{
              glTranslatef(0,0,(maxR-minR)*0.4);
              glRotatef(5,0,1,0);
    }
          glScalef(0.3 * GlobalXSize,GlobalYSize,0.3 * GlobalZSize);
          glutSolidCube(1);
       glPopMatrix();

       glPushMatrix();                /* Gornja poprecna greda. */
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

       glPushMatrix();                /* Donja poprecna greda. */
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

void drawObjects() /* Ovo je zapravo funkcija koja nam iscrtava ovce. Naziv ostao, zato sto je ranije bila i opcija za crtanje kugli. */
{                  /* Obradjuje redom sve ovce iz niza, i za svaku ponaosob od njih poziva funkciju za iscrtavanje ovcica iz fajla sheep.c. */
    int i;
    double raiseBehindMeadow = 1+ ((float)RADIUS/MEADOWDIMENSION_Y); /* Samo da podigne ovcice iznad povrsina livade. */
    for(i=0;i< NumOfSheeps;i++)
    {
            glPushMatrix();
            glTranslatef(Balls[i].pX,raiseBehindMeadow,Balls[i].pZ);
            glScalef(GlobalXSize,GlobalYSize,GlobalZSize);
            drawSheep(Balls[i].angle/180*PI);
            glPopMatrix();
    }
}

void drawBlocks()  /* Funkcija za iscrtavanje zatvorenih povrsina terena tokom igre. U zavisnosti od opsega X i Z koordinata u okviru kojih se    */
{                  /* nalazi slobodna povrsina za ovcice. Ona iscrtava 4 'istegnute' (skalirane) 'kocke' koje ogradjuju slobodnu povrsinu za ovce.*/
    if(minX == -1 && minZ == -1 &&
       maxX == 1  && maxZ ==  1)
        return;

    glTranslatef(0,1.5,0);

    glPushMatrix();     /* Ogradjuje prednji deo slobodne povrsine za ovce. (kraj ka nama) */
       glTranslatef(0,0,(1.0+maxZ) * 0.5);
       glScalef(2,1,1-maxZ);
       glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();     /* Ogradjuje zadnji deo slobodne povrsine. (suprotan kraj od onog ka nama) */
       glTranslatef(0,0,(-1.0 + minZ) * 0.5);
       glScalef(2,1,minZ + 1);
       glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();     /* Ogradjuje desni deo slobodne povrsine. */
       glTranslatef((1 + maxX)*0.5,0,0);
       glScalef((1 - maxX),1,2);
       glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();     /* Ogradjuje levi deo slobodne povrsine. */
       glTranslatef((-1 + minX) * 0.5, 0, 0);
       glScalef(minX + 1,1,2);
       glutSolidCube(1);
    glPopMatrix();
}

/* NOTE: Ovaj kod je iskoriscen iz primera sa vezbi 32_textures. ----------------------------------------------------------------------------------- */
void initializeTexture() /* Funkcija za inicijalizacuje teksture. Koristeci biblioteku "image" ona otvara binarnu .bmp datoteku, */
{                        /* cita njen sarzaj i i funkcijom glGenTextures dodeljuje 'ime' teksturi koje se cuva u globalnoj promenljivoj */
    Image * image;       /*  textureNames i pomocu kog mozemo da podesavamo parametre teksture i kasnije u drugim delovima programa da je koristimo. */
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
} /* ------------------------------------------------------------------------------------------------------------------------------------------------*/

void applyTexture()  /* Funkcija u kojoj primenjujemo inicijalizovanu teksturu na povrsinu gde ovce trce. */
{
    glBindTexture(GL_TEXTURE_2D, textureNames); /* Aktiviramo zeljenu teksturu.*/
    glBegin(GL_QUADS);    /* Kreiramo povrsinu po kojoj ce biti 'zalepljena tekstura. */
                          /* I povezujemo odgovajajuce coskove citave povrsine livade sa coskovima teksture. */
       glTexCoord2f(0, 0);
       glVertex3f(-1.0, 1.1, -1.0);

       glTexCoord2f(1, 0);
       glVertex3f(1.0, 1.1, -1.0);

       glTexCoord2f(1, 1);
       glVertex3f(1.0, 1.1, 1.0);

       glTexCoord2f(0, 1);
       glVertex3f(-1.0, 1.1, 1.0);
    glEnd();


    /* Iskljucujemo aktivnu teksturu */
    glBindTexture(GL_TEXTURE_2D, 0);
}


void set_normal_and_vertex(float u, float v) /* Pomocna funkcija za postavljanje vektora normala. (Neophodno pri parametrizaciji valjka) */
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

void drawScythe()  /* Funkcija koja iscrtava kosu, adaptirana funkcija iz fajla scythe.c (3D-Models). */
{                  /* Nezavisna je od polozaja i dimenzija koje su uklapaju u igru, jer je to sve regulisano u omotac-funkciji drawCloser. */ 
                   /* NOTE: Svesna sam da u kodu ima puno 'magicnih konstanti' (koje nisu pozeljne u praksi), */
    float u, v;    /* pri modelovanju kose su naizmenicno menjani parametri i prevodjen program sve do zeljenog izgleda. */

    glPushMatrix(); /* Prvi deo koda, crtamo drsku kose. (valjak)  */
       glRotatef(10,0,0,1);
       glRotatef(-80,0,1,0);
       glScalef(0.5,0.5,0.5);
       glScalef(0.25,1.2,0.25);
       for (u = 0; u < PI; u += PI / 20) {
           glBegin(GL_TRIANGLE_STRIP);
              for (v = 0; v <= PI*2 + EPSILON; v += PI / 20) {
                 set_normal_and_vertex(u, v);
                 set_normal_and_vertex(u + PI / 20, v);
              }
           glEnd();
        }

       glScalef(1.2,0.1,1.2); /* Drugi deo, crtamo ostar metalni deo. (istegnuta kupa) */
       setLightGrayMaterial();
       for (u = 0; u < PI; u += PI / 20) {
        glBegin(GL_TRIANGLE_STRIP);
        for (v = 0; v <= PI*2 + EPSILON; v += PI / 20) {
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
    glPopMatrix();
}

void drawCloser()  /* Omotac funkcija za crtanje zatvarajuceg objekta. Namerno je koristim iz dva razloga: */
{                  /* Prvo, da bih u njoj podesila parametre kako bi se uklopila u igru i dimenzije terena.(da funkcija bude nezavisna od te igre i samih parametara i bude upotrebljiva negde drugde) */
    glPushMatrix();/* Drugo, da bih lako mogao da se menja zatvarajuci objekat(samo zamenim ime funkcije, na pocetku razvoja je bilo apstrahovano valjkom i funkcijom drawCylinder) */
       glTranslatef(Closer.pX,Closer.pY,Closer.pZ);
       glScalef(0.5*GlobalXSize,GlobalYSize,0.5*GlobalZSize);
       drawScythe(); /* Podeseni parametri, sad pozivamo funkciju za iscrtavanje objekta. */
    glPopMatrix();
}

void drawDecorativeGrass(int translateParameter) /* Funkcija koja iscrtava dekorativnu travu na prednjem i zadnjem kraju terena. */
{
    glScalef(GlobalXSize,GlobalYSize,GlobalZSize);  /* Hocemo da ovde radimo sa svetskim koordinatama. */
    glTranslatef(-MEADOWDIMENSION_X+0.5,2*MEADOWDIMENSION_Y,translateParameter*MEADOWDIMENSION_Z);

    GLdouble equation[] = {0,1,0,0}; /* Jednacina clipping ravni y=0. Njom cemo da isecemo donje delove zarotiranih kocki koje cine travu. */
    glClipPlane(GL_CLIP_PLANE0,equation);
    glEnable(GL_CLIP_PLANE0);

    int i,i_max = 8*2; /* Jedna 'travcica' ce biti siroka osminu dimenzije terena (ta dimenzija je zapravo polovina prave dimenzije), */
    for(i=0;i<i_max;i++) {  /* tj. trebace nam 2 puta toliko da iscrtamo. */
        glPushMatrix();
           glScalef( MEADOWDIMENSION_X / 8.0, 1, 1); /* Istezemo kocku tako da na kraju ide vise u visinu. */
           glRotatef(-45, 0, 0, 1);    /* Rotiramo je za 45 stepeni (zato dobijamo efekat trougla). */
           glutSolidCube(1);          /* Crtamo 'kocku' . */
        glPopMatrix();
        glTranslatef(MEADOWDIMENSION_X/8.0,0,0);  /* Transliramo za njenu sirinu po x - osi za crtanje sledece travke. */
    }
    glDisable(GL_CLIP_PLANE0);
}
