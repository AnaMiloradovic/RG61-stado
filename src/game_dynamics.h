/* Zaglavlje sa funkcijama dinamike same igre. 
   Jedan od najvaznijih header - fajlova za ovu igru. */
#ifndef _GAME_DYNAMICS_
#define _GAME_DYNAMICS_

#define EPSILON (0.01) /* Mala konstanta EPSILON. */ 

#define NOT_TO_CHECK (0) /* Ako se utvrdi kolizija i-te ovce(kugle) sa jos nekim objektom, nije potrebno azurirati smer njene brzine. */
#define TO_CHECK (1)    /*  Ako se utvrdi kolizija i-te ovce(kugle), azurira se njena brzina. */
#define WHOLE_MEADOW_SURFACE (4) /* Povrsina celog terena u kontekstu koordinata X,Z <- [-1,1] */

void rollingBalls(int timer_id);  /* Kretanje ovcica. */
void colissionDetection();        /* Detekcija sudara ovcica i zidova ili ovcica medju sobom. */
void drawHitting();               /* Crtanje pomocne crvene tacke tokom iscrtavanja i zasipanje travom. */ 
void drawHittingPath();           /* Omotac funkcija za iscrtavanje putanje. Ona prvo vrsi potrebne provere. */
   void justDrawHittingPath(POINT* turns, int numberOfTurns); /* Na osnovu tacaka skretanja crta nasu putanju. (kao isprekidanu izlomljenu liniju) */
int stepTest(POINT* turns, int length); /* Test - funkcija da li nam je ovcica nagazila na putanju i da li smo presli direktno preko neke ovcice. */
   int checkBetween(POINT p1, POINT p2, BALL pb); /* Pomocna funkcija za stepTest. */
void checkForNewPoints(POINT* turns, int numberOfTurns); /* Omotac funkcija za proveru novih zagradjenih povrsina i njene 3 pomocne funkcije za proveru. */
   void checkForNewPointsOne(POINT* turns);
   void checkForNewPointsTwo(POINT* turns);
   void checkForNewPointsThree(POINT* turns);
void winning(); /* Funkcija koja se poziva kada igrac pobedi. */
#endif