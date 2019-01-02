#ifndef _DRAW_OBJECTS_
#define _DRAW_OBJECTS_

//Dodatne simbolicke konstante
#define PI (3.141592653589793)
#define EPSILON (0.01)

#define TIMER_LOWER_INTERVAL (5)

#define TIMER_ID_ROLLING (0)
#define TIMER_ID_JUMPING (1)

void drawMeadow();              // Iscrtavanje livade
void drawBalls();              //Iscrtavanje kugli
void drawSheeps();             // Iscrtavanje svih ovaca
void drawClouds();             //Crtanje oblaka
void drawCloser();             // Crtanje objekta iznad livade koji ce zatvarati ovce
                              // TODO: Zameni ih nekim lepsim 3D, za sad ce biti obican valjak
void jumping(int timer_id);
#endif