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
/* ********************* */
extern int hit;
extern char curDir;
extern unsigned char moving;
extern float maxX, minX, maxZ, minZ;
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
            printf("Izasli ste iz igre.");
            exit(0);
            break;
        case 'p':
            on_going = !on_going;
            if(on_going)
                glutPostRedisplay();
            break;
        case ' ':
            if(!hit)
                hit = 1;
            break;
            /* TODO: Dugme na tastaturi kojim 'pucamo' povrsinu koju zatvaramo */
    }
}

void onKeyboardSpecialFunc(int key, int x, int y) {
    glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);
    switch (key) {
        case GLUT_KEY_UP:
            if(Closer.pZ - Closer.vZ >= minZ - 0.1)
               Closer.pZ -= Closer.vZ;
            if (Closer.pZ < minZ)
                Closer.pZ = minZ;
            if(hit && curDir != 'u')
               curDir = 'u';
            break;
        case GLUT_KEY_DOWN:
            if(Closer.pZ + Closer.vZ <= maxZ + 0.1)
               Closer.pZ += Closer.vZ;
            if (Closer.pZ > maxZ)
                Closer.pZ = maxZ;
            if(hit && curDir != 'd')
                curDir = 'd';
            break;
        case GLUT_KEY_LEFT:
            if(Closer.pX - Closer.vX >= minX - 0.1)
                Closer.pX -= Closer.vX;
            if (Closer.pX < minX)
                Closer.pX = minX;
            if(hit && curDir!= 'l')
                curDir = 'l';
             break;
        case GLUT_KEY_RIGHT:
             if(Closer.pX + Closer.vX <= maxX + 0.1)
                Closer.pX += Closer.vX;
             if (Closer.pX > maxX)
                Closer.pX = maxX;
             if(hit && curDir != 'r')
                 curDir = 'r';
            break;
    }

}
