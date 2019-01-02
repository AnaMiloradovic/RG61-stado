#ifndef _GAME_DYNAMICS_
#define _GAME_DYNAMICS_

#define TIMER_INTERVAL (20)
#define TIMER_LOWER_INTEVRAL (10)

#define MAX (30)
void rollingBalls(int timer_id);  // Funkcija kojom implementiramo kretanje kugli
void colissionDetection();     // Funkcija za detekciju sudara
void tryToClose();
#endif