#ifndef _GAME_DYNAMICS_
#define _GAME_DYNAMICS_

#define TIMER_INTERVAL (20)

#define TIMER_ID_ROLLING (0)

void rollingBalls(int timer_id);  // Funkcija kojom implementiramo kretanje kugli
void colissionDetection();     // Funkcija za detekciju sudara
void colissionInField(int collision[]);      // Pomocna funkcija za collisionDetection()

#endif