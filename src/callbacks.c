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
/* HACK: extern int closing;  */
/* ********************* */
extern int hit,hitC, changeD;
extern char curDir;
extern unsigned char moving;
void onReshapeFunction(int w, int h)
{
    width=w;
    height=h;
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

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
        case ' ':
            hit = 1;
            break;
            /* TODO: Dugme na tastaturi kojim 'pucamo' povrsinu koju zatvaramo */
    }
}

void onKeyboardSpecialFunc(int key, int x, int y) {
    glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);
    switch(key)
    {
        case GLUT_KEY_UP:
            Closer.pZ -= Closer.vZ;
            if(Closer.pZ < -1)
                Closer.pZ = -1;
            if(curDir!=0 && curDir != 'u')
                changeD = 1;
            break;
        case GLUT_KEY_DOWN:
            Closer.pZ += Closer.vZ;
            if(Closer.pZ > 1)
               Closer.pZ = 1;
            down = 1;
            if(up | right | left) {
                up = right = left = 0;
                changeD = 1;
            }
            break;
        case GLUT_KEY_LEFT:
            Closer.pX -= Closer.vX;
            if(Closer.pX < -1)
               Closer.pX = -1;
            left = 1;
            if(up | right | down) {
                down = right = up = 0;
                changeD = 1;
            }
             break;
        case GLUT_KEY_RIGHT:
            Closer.pX += Closer.vX;
            if(Closer.pX > 1)
                Closer.pX = 1;
            right = 1;
            if(up | down | left) {
                down = up = left = 0;
                changeD = 1;
            }
            break;

    }
}
