#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "colors.h"
#include "inits.h"
#include "callbacks.h"
#include "game_dynamics.h"

int width,height;
extern int on_going;
extern CLOSER Closer;
extern int closing;
extern unsigned char moving;
void onReshapeFunction(int w, int h)
{
    width=w;
    height=h;
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();             //Cistimo matricu od prethodnog smeca.

    gluPerspective(90,(float) width/height,1,40); //Podesavamo projekciju
}

void onKeyboardFunction(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 27:
            exit(0);
            break;
        case 'p':
            on_going = !on_going;
            if(on_going)
                glutPostRedisplay();
            break;
        case 'j':
            Closer.pX -= Closer.v;
            if(Closer.pX < -1)
                Closer.pX = -1;
            if(closing)
            {
                moving = key;
            }
            break;
        case 'k':
            Closer.pZ += Closer.v;
            if(Closer.pZ > 1)
                Closer.pZ = 1;
            if(closing)
            {
                moving = key;
            }
            break;
        case 'l':
            Closer.pX += Closer.v;
            if(Closer.pX > 1)
                Closer.pX = 1;
            if(closing)
            {
                moving = key;
            }
            break;
        case 'i':
            Closer.pZ -= Closer.v;
            if(Closer.pZ < -1)
                Closer.pZ = -1;
            if(closing)
            {
                moving = key;
            }
            break;
        case ' ':
            moving = ' ';
            if(!closing)
                tryToClose();
            break;
    }
}

