#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "colors.h"
#include "inits.h"
#include "callbacks.h"

int width,height;

void onReshapeFunction(int w, int h)
{
    width=w;
    height=h;
    glViewport(0,0,width,height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();             //Cistimo matricu od prethodnog smeca.
    gluPerspective(90,(float) width/height,1,40); //Podesavamo projekciju
}

