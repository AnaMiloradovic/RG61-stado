#ifndef _GAME_DYNAMICS_
#define _GAME_DYNAMICS_

#define TIMER_INTERVAL (20)
#define TIMER_LOWER_INTEVRAL (10)

#define MAX (4)        // Dozvoljeno je maksimalno 3 prelomne tacke na putanji valjka (+ 1 mesto u nizu za pocetnu tacku iscrtavanja)
void rollingBalls(int timer_id);  // Funkcija kojom implementiramo kretanje kugli
void colissionDetection();     // Funkcija za detekciju sudara
void tryToClose();            // Funkcija pomocu koje iscrtavamo putanju (za sada valjka) kojom zatvaramo novi deo slobodne povrsine
void checkToClosedSurface(POINT turningPoints[], int numOfTurningPoints);  // Funkcija pomocu koje registrujemo eventualne
#endif                                                                     // nove zatvorene delove povrsine