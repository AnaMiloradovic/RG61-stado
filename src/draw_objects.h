/* Zaglavlje sa deklaracijama funkcija za iscrtavanje objekata u igri i funkcija za teksture. */
#ifndef _DRAW_OBJECTS_
#define _DRAW_OBJECTS_

/* Dodatne simbolicke konstante (broj PI i jako mala vrednost EPSILON) */
#define PI (3.141592653589793)
#define EPSILON (0.01)  

void drawMeadow();             /* Iscrtavanje cele livade. */
void drawDecorativeGrass(int translateParameter); /* Dekorativna trava. */
void drawObjects();            /* Crtanje svih ovcica. */
void drawCloser();             /* Crtanje objekta iznad livade koji ce zatvarati ovce. */
void drawScythe();             /* Crtanje kose. (koja je zatvarajuci objekat) */
void drawHedge(float minR, float maxR,float fixed, int indX); /* Crtanje ogradice oko ovcica na pobednickoj sceni. */
   void set_normal_and_vertex(float u, float v); /* Pomocna podfunkcija za postavljanje vektora normala.(za crtanje valjka) */
void drawBlocks();             /* Crtanje zatvorenih povrsina. */
void initializeTexture();      /* Inicijalizacija teksture. */
void applyTexture();           /* Primena teksture na povrsinu na kojoj trce ovcice. */
#endif
