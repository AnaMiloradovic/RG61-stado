#ifndef _GAME_DYNAMICS_
#define _GAME_DYNAMICS_

#define TIMER_INTERVAL (20)

void rollingBalls(int timer_id);  // Funkcija kojom implementiramo kretanje kugli
void colissionDetection();     // Funkcija za detekciju sudara
void drawHitting();
void drawHittingPath();
int stepTest(POINT beginPoint, POINT* turns, int length);
static int checkBetween(POINT p1, POINT p2, BALL pb);
void testForNewPoints(POINT beginPoint, POINT* turns, int length);
#endif