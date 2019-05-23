#ifndef _GAME_DYNAMICS_
#define _GAME_DYNAMICS_

#define TIMER_INTERVAL (20)

void rollingBalls(int timer_id);  // Funkcija kojom implementiramo kretanje kugli
void colissionDetection();     // Funkcija za detekciju sudara
void drawHitting();
void drawHittingPath();
static void inline justDrawHittingPath(POINT* turns, int numberOfTurns);
int stepTest(POINT* turns, int length);
static int checkBetween(POINT p1, POINT p2, BALL pb);
void checkForNewPoints(POINT* turns, int numberOfTurns);
   void checkForNewPointsOne(POINT* turns);
   void checkForNewPointsTwo(POINT* turns);
   void checkForNewPointsThree(POINT* turns);
void winning();
#endif