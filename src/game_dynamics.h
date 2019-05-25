#ifndef _GAME_DYNAMICS_
#define _GAME_DYNAMICS_

#define TIMER_INTERVAL (20)
#define EPSILON (0.01)  

#define NOT_TO_CHECK (0) // TODO Ako se utvrdi kolizija i-te ovce(kugle), nije potrebno azurirati smer njene brzine.
#define TO_CHECK (1)    //TODO  Ako se utvrdi kolizija i-te ovce(kugle), azurira se njena brzina.
#define WHOLE_MEADOW_SURFACE (4) /* Povrsina celog terena u kontekstu koordinata X,Z <- [-1,1] */

void rollingBalls(int timer_id);  // Funkcija kojom implementiramo kretanje kugli
void colissionDetection();     // Funkcija za detekciju sudara
void drawHitting();
void drawHittingPath();
void justDrawHittingPath(POINT* turns, int numberOfTurns);
int stepTest(POINT* turns, int length);
int checkBetween(POINT p1, POINT p2, BALL pb);
void checkForNewPoints(POINT* turns, int numberOfTurns);
   void checkForNewPointsOne(POINT* turns);
   void checkForNewPointsTwo(POINT* turns);
   void checkForNewPointsThree(POINT* turns);
void winning();
#endif