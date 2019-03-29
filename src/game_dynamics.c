#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "colors.h" //Ovde cuvamo boje koje koristimo u igri
#include "inits.h" //Ovde nam se nalaze deklaracije inicijalizacionih funkcija
#include "callbacks.h" //Callback funckije
#include "draw_objects.h" //Funckije za iscrtavanje objekata
#include "light_and_materials.h" //Funkcije za podesavanje osvetljenja i postavljanje materijala
#include "game_dynamics.h" //Funckije za kretanja u igri

extern BALL Balls[];
extern CLOSER Closer;
extern int ifJustSheeps;
extern int NumOfSheeps;
extern float minX,maxX,minZ,maxZ;
int on_going = 1;
/* int closing = 0;  */
/* TODO Da se implementira */
extern unsigned int pointsWon;
extern float coveredPercent;
/* **************************************** */
int hit = 0;
char curDir = 0;


/* **************************************** */
unsigned char moving;

extern double GlobalXSize;
extern double GlobalYSize;
extern double GlobalZSize;

void rollingBalls(int timer_id)
{
    if(timer_id != TIMER_ID_ROLLING)
        return;
    int i;
    for(i=0;i<NumOfSheeps;i++)
    {
        Balls[i].pX += Balls[i].vX;
        Balls[i].pZ += Balls[i].vZ;
        if(!ifJustSheeps)
          Balls[i].angle += Balls[i].w_angle;
    }
    
    if(!ifJustSheeps)
    {
       if(Balls[i].angle > 360)
          Balls[i].angle -= 360;
       else if(Balls[i].angle < -360)
          Balls[i].angle += 360;
    }
    colissionDetection();



    glutPostRedisplay();
}

/* TODO : Ispraviti funkcije za zatvaranje i iscrtavanje kocki i implementirati bodove */


void colissionDetection()
{
    int i,j,colission[]={TO_CHECK,TO_CHECK,TO_CHECK,TO_CHECK,TO_CHECK};
    for(i=0;i<NumOfSheeps;i++)
    {
        if(Balls[i].pX + ((float)RADIUS/MEADOWDIMENSION_X) >= maxX || Balls[i].pX  - ((float)RADIUS/MEADOWDIMENSION_X) <= minX)
        {
            Balls[i].vX = -Balls[i].vX;
            Balls[i].w_angle = - Balls[i].w_angle;
            colission[i] = NOT_TO_CHECK;
        }
        if(Balls[i].pZ + ((float)RADIUS/MEADOWDIMENSION_Z) >= maxZ || Balls[i].pZ  - ((float)RADIUS/MEADOWDIMENSION_Z) <= minZ)
        {
            Balls[i].vZ = -Balls[i].vZ;
            Balls[i].w_angle = - Balls[i].w_angle;
            colission[i] = NOT_TO_CHECK;
        }
    }
    
    for(i=0;i<NumOfSheeps;i++)
        for(j=i+1;j<NumOfSheeps;j++)
        {
            if(MEADOWDIMENSION_X*MEADOWDIMENSION_X*(Balls[i].pX-Balls[j].pX)*(Balls[i].pX-Balls[j].pX) +
                 MEADOWDIMENSION_Z*MEADOWDIMENSION_Z*(Balls[i].pZ-Balls[j].pZ)*(Balls[i].pZ-Balls[j].pZ) - RADIUS*RADIUS*2*2 < EPSILON)  
            {
                if(colission[i])
                {
                   Balls[i].vX = -Balls[i].vX;
                   Balls[i].vZ = -Balls[i].vZ;
                   Balls[i].w_angle = - Balls[i].w_angle;
                   colission[i]= NOT_TO_CHECK;
                }
                if(colission[j])
                {
                   Balls[j].vX = -Balls[j].vX;
                   Balls[j].vZ = -Balls[j].vZ;
                   Balls[j].w_angle = - Balls[j].w_angle;
                   colission[j] = NOT_TO_CHECK;
                }
            }
        }
}

void drawHitting() /* TODO: Da se popravi da lepse izgleda */
{
    glDisable(GL_LIGHTING);
    glColor3f(1,0,0);
    glBegin(GL_LINES);
       glVertex3f(Closer.pX,Closer.pY,Closer.pZ);
       glVertex3f(Closer.pX, MEADOWDIMENSION_Y + 0.001 , Closer.pZ);
    glEnd();
    glEnable(GL_LIGHTING);
}

void drawHittingPath()
{
    static char currentDir = 0;
    static POINT beginPoint; /* Pocetna tacka putanje */
    static POINT turns[3]; /* Maks 3 skretanja */
    static int  numberOfTurns = 0,beginInd = 1;
    static float beginExtrem = 0;
    int i;
    if(beginInd)
    {
        if(Closer.pX != minX && Closer.pX != maxX
        && Closer.pZ != minZ && Closer.pZ != maxZ)
        {
            hit = 0;
            return;
        }
        beginInd = 0;
        beginPoint.pX = Closer.pX;
        beginPoint.pY =1.1;
        beginPoint.pZ = Closer.pZ;
        return;
    }
    if(curDir != currentDir && currentDir == 0)
       currentDir = curDir;
    if(curDir != currentDir)
    {
        if((curDir == 'u' && currentDir == 'd') || (curDir == 'd' && currentDir == 'u')
        || (curDir == 'l' && currentDir == 'r') || (curDir == 'r' && currentDir == 'l'))
        {
            currentDir =  curDir = 0;
            numberOfTurns = 0;
            hit = 0;
            beginInd = 1;
            return;
        }
        currentDir = curDir;
        if(numberOfTurns >= 3)
        {
            currentDir =  curDir = 0;
            numberOfTurns = 0;
            hit = 0;
            beginInd = 1;
            return;
        }
        turns[numberOfTurns].pX = Closer.pX;
        turns[numberOfTurns].pY = 1.1;
        turns[numberOfTurns].pZ = Closer.pZ;
        if(currentDir == 'r')
            turns[numberOfTurns].pX -= Closer.vX;
        else if(currentDir == 'l')
            turns[numberOfTurns].pX += Closer.vX;
        else if(currentDir == 'd')
            turns[numberOfTurns].pZ -= Closer.vZ;
        else if(currentDir == 'u')
            turns[numberOfTurns].pZ += Closer.vZ;
        numberOfTurns++;
    }

    if(stepTest(beginPoint,turns,numberOfTurns))
    {
        hit = 0;
        beginInd = 1;
        currentDir = curDir = 0;
        numberOfTurns = 0;
        return;
    }

    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glColor3f(1,0,0);
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(5, 0xf0f0);
    glBegin(GL_LINE_STRIP);
    glVertex3f(beginPoint.pX,beginPoint.pY,beginPoint.pZ);
    for(i=0;i<numberOfTurns;i++)
       glVertex3f(turns[i].pX,turns[i].pY,turns[i].pZ);
    glVertex3f(Closer.pX,1.1,Closer.pZ);
    glEnd();
    glDisable(GL_LINE_STIPPLE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    if((Closer.pX <= minX || Closer.pX >= maxX
        || Closer.pZ <= minZ || Closer.pZ >= maxZ)
       && (beginPoint.pX != Closer.pX || beginPoint.pZ != Closer.pZ)){
        testForNewPoints(beginPoint, turns, numberOfTurns);
        hit = 0;
        currentDir = 0;
        curDir = 0;
        numberOfTurns = 0;
        beginInd = 1;
        return;
    }
}

int stepTest(POINT beginPoint ,POINT* turns, int length)
{
    POINT closerPosition;
    closerPosition.pX = Closer.pX;
    closerPosition.pY = 1.1;
    closerPosition.pZ = Closer.pZ;
    int i;
    for(i=0;i<NumOfSheeps;i++)
    {
        if(fabs((double) Closer.pX - Balls[i].pX) < 0.25*RADIUS*GlobalXSize && fabs((double) Closer.pZ - Balls[i].pZ) < 0.25*RADIUS*GlobalZSize)
        {
            printf("Presli ste preko ovce\n\nKRAJ IGRE\n");
            exit(EXIT_SUCCESS);
        }
       if(checkBetween(beginPoint,turns[0],Balls[i]))
           return 1;
       int j;
       for(j=0;j<length-1;j++) {
           if (checkBetween(turns[j], turns[j + 1], Balls[i]))
               return 1;
       }
           if(checkBetween(turns[length-1],closerPosition,Balls[i]))
               return 1;
    }
    return 0;
}

static int inline checkBetween(POINT p1, POINT p2, BALL pb)
{
    if(p1.pX == p2.pX && fabs((double)p1.pX - pb.pX) < 0.1*GlobalXSize){
        if((p1.pZ <= pb.pZ && p2.pZ >= pb.pZ) || (p1.pZ >= pb.pZ && p2.pZ <= pb.pZ))
            return 1;}
    else if(p1.pZ == p2.pZ && fabs((double)p1.pZ - pb.pZ) < 0.1*GlobalZSize){
        if((p1.pX <= pb.pX && p2.pX >= pb.pX) || (p1.pX >= pb.pX && p2.pX <= pb.pX))
            return 1;}
    return 0;

}

void testForNewPoints(POINT beginPoint, POINT* turns, int length)
{

    if(length == 0)
    {
        int i, indX, indL;
        if(Closer.pX == maxX || Closer.pX == minX) /* TODO: Iskomentarisi ga */
            indX = 0;
        else indX = 1;
        if(!indX)
        {
            if(Balls[0].pZ < Closer.pZ)
               indL = 1;
            else indL = 0;
        } else{
            if(Balls[0].pX < Closer.pX)
                indL = 1;
            else indL = 0;
        }

        for(i=1;i<NumOfSheeps;i++)
        {
            if(!indX)
            {
                if(indL && ( Balls[i].pZ >= Closer.pZ || Balls[i].pZ + 2*RADIUS * GlobalZSize >= Closer.pZ)) {
                    printf("VecaZ");
                    return;
                }
                else if(!indL && ( Balls[i].pZ <= Closer.pZ || Balls[i].pZ - 2*RADIUS * GlobalZSize <= Closer.pZ)) {
                    printf("ManjaZ");
                    return;
                }
            }
            else{
                if(indL && (Balls[i].pX >= Closer.pX || Balls[i].pX + 2*RADIUS * GlobalXSize >= Closer.pX))
                {
                    printf("VecaX");
                    return;
                }

                else if(!indL && (Balls[i].pX <= Closer.pX || Balls[i].pX - 2*RADIUS * GlobalXSize >= Closer.pX))
                {
                    printf("ManjaX");
                    return;
                }
            }
        }

        if(indL && !indX) {
            printf("novi maks z");

            maxZ = Closer.pZ;
        }
        else if(indL && indX) {
            printf("novi maks x");

            maxX = Closer.pX;
        }
        else if(!indL && !indX) {
            printf("novi min z");
            minZ = Closer.pZ;
        }
        else if(!indL && indX) {
            minX = Closer.pX;
            printf("novi min x");
        }

    }

    if(length == 1)
    {
        int i,indX,indL;
        if(beginPoint.pX == turns[0].pX)
        {
            indX = 1;
            if(Balls[0].pX < beginPoint.pX) {
                indL = 1;
                if(Closer.pX > minX)
                    return;
            }
            else
            {
                indL = 0;
                if(Closer.pX < maxX)
                    return;
            }
        } else {
            indX = 0;
            if (Balls[0].pZ < beginPoint.pZ) {
                indL = 1;
                if (Closer.pZ > minZ)
                    return;
            }
            else{
                indL = 0;
                if(Closer.pZ < maxZ)
                    return;
            }
        }
        for(i=0;i<NumOfSheeps;i++)
        {
            if(indX && indL && ( Balls[i].pX >= beginPoint.pX
            || (beginPoint.pZ == minZ && ( Balls[i].pZ >= turns[0].pZ || Balls[i].pZ + 2*RADIUS * GlobalZSize >= turns[0].pZ))
            || (beginPoint.pZ == maxZ && ( Balls[i].pZ <= turns[0].pZ || Balls[i].pZ - 2*RADIUS * GlobalZSize <= turns[0].pZ))))
                return;
            if(indX && !indL && ( Balls[i].pX <= beginPoint.pX
               || (beginPoint.pZ == minZ && ( Balls[i].pZ >= turns[0].pZ || Balls[i].pZ + 2*RADIUS * GlobalZSize >= turns[0].pZ))
               || (beginPoint.pZ == maxZ && ( Balls[i].pZ <= turns[0].pZ || Balls[i].pZ - 2*RADIUS * GlobalZSize <= turns[0].pZ))))
                return;
            if(!indX && indL && (Balls[i].pZ >= beginPoint.pZ
               || (beginPoint.pX == minX && (Balls[i].pX >= turns[0].pX || Balls[i].pX + 2*RADIUS * GlobalXSize >= turns[0].pX))
               || (beginPoint.pX == maxX && (Balls[i].pX <= turns[0].pX || Balls[i].pX - 2*RADIUS * GlobalXSize <= turns[0].pX))))
                return;
            if(!indX && !indL && ( Balls[i].pZ <= beginPoint.pZ
               || (beginPoint.pX == minX && (Balls[i].pX >= turns[0].pX || Balls[i].pX + 2*RADIUS * GlobalXSize >= turns[0].pX))
               || (beginPoint.pX == maxX && (Balls[i].pX <= turns[0].pX || Balls[i].pX - 2*RADIUS * GlobalXSize <= turns[0].pX))))
                return;
        }

        float min1X = minX , min1Z = minZ , max1X = maxX , max1Z = maxZ;
        if(beginPoint.pX == min1X || (indX && indL))
            maxX = turns[0].pX;
        if(beginPoint.pX == max1X || (indX && !indL))
            minX = turns[0].pX;
        if(beginPoint.pZ == min1Z || (!indX && indL))
            maxZ = turns[0].pZ;
        if(beginPoint.pZ == max1Z || (!indX && !indL))
            minZ = turns[0].pZ;

    }

    if(length == 2){
        int i,indL,indX;
        float otherBoundL,otherBoundG;
        if(beginPoint.pX == Closer.pX)
        {
            indX = 1;
            if(beginPoint.pX == minX)
                indL = 1;
            else indL = 0;
            if(beginPoint.pZ < Closer.pZ)
            {
                otherBoundL = beginPoint.pZ;
                otherBoundG = Closer.pZ;
            } else{
                otherBoundG = beginPoint.pZ;
                otherBoundL= Closer.pZ;
            }
        }
        else {
            indX = 0;
            if(beginPoint.pZ == minZ)
                indL = 1;
            else indL = 0;
            if(beginPoint.pX < Closer.pX)
            {
                otherBoundL = beginPoint.pX;
                otherBoundG = Closer.pX;
            } else{
                otherBoundG = beginPoint.pX;
                otherBoundL= Closer.pX;
            }
        }

        for(i=0;i<NumOfSheeps;i++)
        {
            if(indX && indL && (Balls[i].pX >= turns[0].pX || Balls[i].pX + 2*RADIUS * GlobalXSize >= turns[0].pX) )
                return;
            if(indX && !indL && (Balls[i].pX <= turns[0].pX || Balls[i].pX - 2*RADIUS * GlobalXSize <= turns[0].pX) )
                return;
            if(!indX && indL && (Balls[i].pZ >= turns[0].pZ || Balls[i].pZ + 2*RADIUS * GlobalZSize >= turns[0].pZ))
                return;
            if(!indX && !indL && (Balls[i].pZ <= turns[0].pZ || Balls[i].pZ - 2*RADIUS * GlobalZSize <= turns[0].pZ))
                return;
            if(indX && (Balls[i].pZ - 2*RADIUS * GlobalZSize <= otherBoundL || Balls[i].pZ + 2*RADIUS * GlobalZSize >= otherBoundG))
                return;
            if(!indX && (Balls[i].pX - 2*RADIUS * GlobalXSize <= otherBoundL || Balls[i].pX + 2*RADIUS * GlobalXSize >= otherBoundG))
                return;
        }

        if(indX)
        {
            if(indL)
                maxX = turns[0].pX;
            else minX = turns[0].pX;
            minZ = otherBoundL;
            maxZ = otherBoundG;
        }

        else
        {
            if(indL)
                maxZ = turns[0].pZ;
            else minZ = turns[0].pZ;
            minX = otherBoundL;
            maxX = otherBoundG;
        }
    }

}
