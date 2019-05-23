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
int winner = 0;
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

void drawHitting()
{
    int i;
    static int even_odd = 0, max_cur = 1, max_t = 10;
    float step = 1;
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glColor3f(1, 0, 0);
    glPointSize(2);
    glBegin(GL_POINTS);
    glVertex3f(Closer.pX, 1.1, Closer.pZ);
    glEnd();
    glColor3f(0, 0.78, 0);
    glPointSize(2.5);
    glBegin(GL_POINTS);
    for (i = 0; i < max_cur; i++)
        glVertex3f(Closer.pX - 0.15, Closer.pY - even_odd * 0.5 * step - i * step - 1.3, Closer.pZ);
    //glVertex3f(Closer.pX - 0.05,Closer.pY- even_odd*0.5*step - i*step - 0.001  , Closer.pZ);
    glEnd();
    if (max_cur < max_t)
        max_cur++;
    if (even_odd)
        even_odd = 0;
    else even_odd++;

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
}
void drawHittingPath()
{
    static int firstCall = 1;
    static POINT turns[4];
    static int numberOfTurns = 0;
    static char currentDir = 0;
    if(firstCall)
    {
       if( Closer.pX != minX &&
           Closer.pX != maxX &&
           Closer.pZ != minZ &&
           Closer.pZ != maxZ)
        {
            hit = 0;
            return;
        }
       turns[0].pX = Closer.pX;
        turns[0].pY = 1.1;
        turns[0].pZ = Closer.pZ;

        numberOfTurns = 1;

        currentDir = curDir;
        firstCall = 0;
       return;
    }

    else if (currentDir != 0 && curDir != currentDir)
    {
        if((curDir == 'u' && currentDir == 'd') ||
           (curDir == 'd' && currentDir == 'u') ||
           (curDir == 'l' && currentDir == 'r') ||
           (curDir == 'r' && currentDir == 'l'))
        {
            firstCall = 1;
            curDir = 0;
            currentDir = 0;
            numberOfTurns = 0;
            hit = 0;
        }
        if(numberOfTurns == 4)
        {
            firstCall = 1;
            curDir = 0;
            currentDir = 0;
            numberOfTurns = 0;
            hit = 0;
        }
        turns[numberOfTurns].pX = Closer.pX;
        turns[numberOfTurns].pY = 1.1;
        turns[numberOfTurns].pZ = Closer.pZ;

        switch(curDir)
        {
            case 'l':
                turns[numberOfTurns].pX += Closer.vX;
                break;
            case 'r':
                turns[numberOfTurns].pX -= Closer.vX;
                break;
            case 'u':
                turns[numberOfTurns].pZ += Closer.vZ;
                break;
            case 'd':
                turns[numberOfTurns].pZ -= Closer.vZ;
                break;
        }
        numberOfTurns++;

        currentDir = curDir;
    }

    if((currentDir == 0 && ( Closer.pX < minX || Closer.pX > maxX ||
        Closer.pZ < minZ || Closer.pZ > maxZ)) ||
        stepTest(turns,numberOfTurns))
    {
        firstCall = 1;
        curDir = 0;
        currentDir = 0;
        numberOfTurns = 0;
        hit = 0;
    }

    if(currentDir != 0 && ( Closer.pX >= maxX ||
           Closer.pX <= minX ||
            Closer.pZ >= maxZ ||
           Closer.pZ <= minZ))
    {
        if( (Closer.pX >= maxX && curDir == 'r' && Closer.pX != turns[0].pX) ||
            (Closer.pX <= minX && curDir == 'l' && Closer.pX != turns[0].pX) ||
            (Closer.pZ >= maxZ && curDir == 'd' && Closer.pZ != turns[0].pZ) ||
            (Closer.pZ <= minZ && curDir == 'u' && Closer.pZ != turns[0].pZ))
           checkForNewPoints(turns,numberOfTurns);
        firstCall = 1;
        curDir = 0;
        currentDir = 0;
        numberOfTurns = 0;
        hit = 0;
    }

    if(currentDir == 0)
        currentDir = curDir;
    justDrawHittingPath(turns,numberOfTurns);

}

static void inline justDrawHittingPath(POINT* turns, int numberOfTurns)
{
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glColor3f(.4,0.78,.4);

    glEnable(GL_LINE_STIPPLE);
    glLineStipple(5, 0xf0f0);
    glBegin(GL_LINE_STRIP);
    int i;
    for(i=0;i<numberOfTurns;i++)
        glVertex3f(turns[i].pX,turns[i].pY,turns[i].pZ);
    glVertex3f(Closer.pX,1.1,Closer.pZ);
    glEnd();
    glDisable(GL_LINE_STIPPLE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}
/*
void drawHittingPath()
{
    static char currentDir = 0;
    static POINT beginPoint;  Pocetna tacka putanje
   static POINT turns[3];  Maks 3 skretanja
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
    glColor3f(.4,0.78,.4);

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
}*/

int stepTest(POINT* turns, int length)
{
    POINT closerPosition;
    closerPosition.pX = Closer.pX;
    closerPosition.pY = 1.1;
    closerPosition.pZ = Closer.pZ;
    int i;
    for(i=0;i<NumOfSheeps;i++)
    {
        if(fabs((double) Closer.pX - Balls[i].pX) <  RADIUS*GlobalXSize && fabs((double) Closer.pZ - Balls[i].pZ) < RADIUS*GlobalZSize)
        {
            printf("Presli ste preko ovce\n\nKRAJ IGRE\n");
            exit(EXIT_SUCCESS);
        }
       int j;
       for(j=0;j<length;j++) {
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
    if(p1.pX == p2.pX && fabs((double)p1.pX - pb.pX) < RADIUS*GlobalXSize){
        if((p1.pZ <= pb.pZ && p2.pZ >= pb.pZ) || (p1.pZ >= pb.pZ && p2.pZ <= pb.pZ))
            return 1;}
    else if(p1.pZ == p2.pZ && fabs((double)p1.pZ - pb.pZ) < RADIUS*GlobalZSize){
        if((p1.pX <= pb.pX && p2.pX >= pb.pX) || (p1.pX >= pb.pX && p2.pX <= pb.pX))
            return 1;}
    return 0;

}

void checkForNewPoints(POINT* turns, int numberOfTurns)
{
    switch(numberOfTurns)
    {
        case 1:
            checkForNewPointsOne(turns);
            break;
        case 2:
            checkForNewPointsTwo(turns);
            break;
        case 3:
            checkForNewPointsThree(turns);
            break;
    }
    printf("Covered percent %.2lf%% \n",coveredPercent);
    if(coveredPercent > 100 - NumOfSheeps * PERCENT_PER_SHEEP)
    {
        printf("Pobeda! BRAVOOO!!!\n");
        winning();
    }
}

void checkForNewPointsOne(POINT* turns)
{
    char check_option, side;
    if(turns[0].pX == Closer.pX)
        check_option = 'x';
    else check_option = 'z';
    if(check_option == 'x')
    {
        if(Balls[0].pX < Closer.pX)
           side = 'l';
        else side = 'u';
    }
    else{
        if(Balls[0].pZ < Closer.pZ)
            side = 'l';
        else side = 'u';
    }
    int i;
    for(i=1;i<NumOfSheeps;i++)
    {
        if(check_option == 'x' && side == 'l' && Balls[i].pX >= Closer.pX)
            return;
        else if(check_option == 'x' && side == 'u' && Balls[i].pX <= Closer.pX)
            return;
        else if(check_option == 'z' && side == 'l' && Balls[i].pZ >= Closer.pZ)
            return;
        else if(check_option == 'z' && side == 'u' && Balls[i].pZ <= Closer.pZ)
            return;
    }

    if(check_option == 'x' && side == 'l')
    {
        coveredPercent += 100*(((maxX - Closer.pX)*(maxZ - minZ)) / 4.0);
        pointsWon += (maxX - Closer.pX) / (maxX - minX);
        maxX = Closer.pX;
    }
    else if(check_option == 'x' && side == 'u')
    {
        coveredPercent += 100*(((Closer.pX - minX) * (maxZ - minZ)) / 4.0);
        pointsWon += (Closer.pX - minX) / (maxX - minX);
        minX = Closer.pX;
    }
    else if(check_option == 'z' && side == 'l')
    {
        coveredPercent += 100*(((maxZ - Closer.pZ)*(maxX- minX)) / 4.0);
        pointsWon += (maxZ - Closer.pZ) / (maxZ - minZ);
        maxZ = Closer.pZ;
    }
    else if(check_option == 'z' && side == 'u')
    {
        coveredPercent += 100*(((Closer.pZ -  minZ)*(maxX- minX)) / 4.0);
        pointsWon += (Closer.pZ - minZ) / (maxZ - minZ);
        minZ = Closer.pZ;
    }
}

void checkForNewPointsTwo(POINT* turns)
{
     float check_x = turns[1].pX, check_z = turns[1].pZ;
     char x_side, z_side;
     if(turns[0].pX == minX)
     {
         x_side = 'l';
         if(Closer.pZ > turns[1].pZ)
             z_side = 'u';
         else z_side = 'l';
     }
     else if(turns[0].pX == maxX)
     {
         x_side = 'u';
         if(Closer.pZ > turns[1].pZ)
             z_side = 'u';
         else z_side = 'l';
     }
     else if(turns[0].pZ == minZ)
     {
         z_side = 'l';
         if(Closer.pX > turns[1].pX)
             x_side = 'u';
         else x_side = 'l';
     }
     else if(turns[0].pZ == maxZ)
     {
         z_side = 'u';
         if(Closer.pX > turns[1].pX)
             x_side = 'u';
         else x_side = 'l';
     }

     int i;
     for(i=0;i<NumOfSheeps;i++)
     {
         if(x_side == 'l' && z_side == 'l')
         {
             if(Balls[i].pX >= check_x || Balls[i].pZ >= check_z)
                 return;
         }
         if(x_side == 'l' && z_side == 'u')
         {
             if(Balls[i].pX >= check_x || Balls[i].pZ <= check_z)
                 return;
         }
         if(x_side == 'u' && z_side == 'l')
         {
             if(Balls[i].pX <= check_x || Balls[i].pZ >= check_z)
                 return;
         }
         if(x_side == 'u' && z_side == 'u')
         {
             if(Balls[i].pX <= check_x || Balls[i].pZ <= check_z)
                 return;
         }
     }
    if(x_side == 'l' && z_side == 'l')
    {
        coveredPercent += 100.0*(((maxZ - turns[1].pZ)*(maxX - minX) + (maxX - turns[1].pX)*(turns[1].pZ - minZ))/4.0);
        maxX = check_x;
        maxZ = check_z;
    }
    if(x_side == 'l' && z_side == 'u')
    {
        coveredPercent += 100.0*(((turns[1].pZ - minZ)*(maxX-minX) + (maxZ - turns[1].pZ)*(maxX - turns[1].pX) )/4.0);
        maxX = check_x;
        minZ = check_z;
    }
    if(x_side == 'u' && z_side == 'l')
    {
        coveredPercent += 100.0*(((maxZ - turns[1].pZ)*(maxX - minX) + (turns[1].pZ - minZ)*(turns[1].pX - minX) )/4.0);
        minX = check_x;
        maxZ = check_z;
    }
    if(x_side == 'u' && z_side == 'u')
    {
        coveredPercent += 100*(((turns[1].pZ - minZ)*(maxX - minX) + (turns[1].pX - minX) * (maxZ - turns[1].pZ))/4.0);
        minX = check_x;
        minZ = check_z;
    }

}

void checkForNewPointsThree(POINT* turns)
{
    if(turns[0].pX != Closer.pX && turns[0].pZ != Closer.pZ)
        return;
    char check_parameter, side, range;

    if(turns[0].pX == minX)
    {
        check_parameter = 'x';
        side = 'l';
        if(turns[1].pZ < turns[2].pZ)
            range = 'a';
        else range = 'd';
    }
    else if(turns[0].pX == maxX)
    {
        check_parameter = 'x';
        side = 'u';
        if(turns[1].pZ < turns[2].pZ)
            range = 'a';
        else range = 'd';
    }
    else if(turns[0].pZ == minZ)
    {
        check_parameter = 'z';
        side = 'l';
        if(turns[1].pX < turns[2].pX)
            range = 'a';
        else range = 'd';
    }
    else if(turns[0].pZ == maxZ)
    {
        check_parameter = 'z';
        side = 'u';
        if(turns[1].pX < turns[2].pX)
            range = 'a';
        else range = 'd';
    }
    int i;
    for(i=0;i<NumOfSheeps;i++)
    {
        if(check_parameter == 'x')
        {
            if(side == 'l' && Balls[i].pX >= turns[1].pX)
                return;
            else if(side == 'u' && Balls[i].pX <= turns[1].pX)
                return;
            if(range == 'a' && ( Balls[i].pZ <= turns[1].pZ || Balls[i].pZ >= turns[2].pZ ) )
                return;
            else if(range == 'd' && (Balls[i].pZ >= turns[1].pZ || Balls[i].pZ <= turns[2].pZ))
                return;
        }
        if(check_parameter == 'z')
        {
            if(side == 'l' && Balls[i].pZ >= turns[1].pZ)
                return;
            else if(side == 'u' && Balls[i].pZ <= turns[1].pZ)
                return;
            if(range == 'a' && ( Balls[i].pX <= turns[1].pX || Balls[i].pX >= turns[2].pX ) )
                return;
            else if(range == 'd' && (Balls[i].pX >= turns[1].pX || Balls[i].pX <= turns[2].pX))
                return;
        }
    }

    if(check_parameter == 'x')
    {
        if(side == 'l')
            maxX = turns[1].pX;
        else minX = turns[1].pX;
        if(range == 'a')
        {
            minZ = turns[1].pZ;
            maxZ = turns[2].pZ;
        }else{
            minZ = turns[2].pZ;
            maxZ = turns[1].pZ;
        }
    }else{
        if(side == 'l')
            maxZ = turns[1].pZ;
        else minZ = turns[1].pZ;
        if(range == 'a')
        {
            minX = turns[1].pX;
            maxX = turns[2].pX;
        }else{
            minX = turns[2].pX;
            maxX = turns[1].pX;
        }
    }

}

void winning()
{
    on_going = 0;
    winner = 1;
    printf("Svaka cast!\n");
    glutPostRedisplay();
}
/*
void testForNewPoints(POINT beginPoint, POINT* turns, int length)
{

    if(length == 0)
    {
        int i, indX, indL;
        if(Closer.pX == maxX || Closer.pX == minX)*/ /* TODO: Iskomentarisi ga */
        /*    indX = 0;
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
                if(indL && ( Balls[i].pZ >= Closer.pZ || Balls[i].pZ + 4*RADIUS * GlobalZSize >= Closer.pZ)) {
                    printf("VecaZ ");
                    return;
                }
                else if(!indL && ( Balls[i].pZ <= Closer.pZ || Balls[i].pZ - 4*RADIUS * GlobalZSize <= Closer.pZ)) {
                    printf("ManjaZ ");
                    return;
                }
            }
            else{
                if(indL && (Balls[i].pX >= Closer.pX || Balls[i].pX + 4*RADIUS * GlobalXSize >= Closer.pX))
                {
                    printf("VecaX ");
                    return;
                }

                else if(!indL && (Balls[i].pX <= Closer.pX || Balls[i].pX - 4*RADIUS * GlobalXSize >= Closer.pX))
                {
                    printf("ManjaX ");
                    return;
                }
            }
        }

        float addPercent = 0.0;
        if(indL && !indX) {
            printf("novi maks z ");

            float oldValue = maxZ;
            maxZ = Closer.pZ;
            addPercent = (oldValue - maxZ) * (maxX - minX) * 0.25 * 100.0;
        }
        else if(indL && indX) {
            printf("novi maks x ");
            float oldValue = maxX;
            maxX = Closer.pX;
            addPercent = (oldValue - maxX) * (maxZ - minZ) * 0.25 * 100.0;
        }
        else if(!indL && !indX) {
            printf("novi min z ");
            float oldValue = minZ;
            minZ = Closer.pZ;
            addPercent = (minZ - oldValue) * (maxX - minX) * 0.25 * 100.0;
        }
        else if(!indL && indX) {
            float oldValue = minX;
            minX = Closer.pX;
            addPercent = (minX - oldValue) * (maxZ - minZ) * 0.25 * 100.0;
            printf("novi min x ");

        }
        printf("Added percent + %.2f%%\n",addPercent);

        coveredPercent += addPercent;
        pointsWon += (int) 100.0 * addPercent;
        printf("Points: %d\n",pointsWon);
    }

    if(length == 1)
    {
        float addPercent = 0.0;
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
            || (beginPoint.pZ == minZ && ( Balls[i].pZ >= turns[0].pZ || Balls[i].pZ + 4*RADIUS * GlobalZSize >= turns[0].pZ))
            || (beginPoint.pZ == maxZ && ( Balls[i].pZ <= turns[0].pZ || Balls[i].pZ - 4*RADIUS * GlobalZSize <= turns[0].pZ))))
                return;
            if(indX && !indL && ( Balls[i].pX <= beginPoint.pX
               || (beginPoint.pZ == minZ && ( Balls[i].pZ >= turns[0].pZ || Balls[i].pZ + 4*RADIUS * GlobalZSize >= turns[0].pZ))
               || (beginPoint.pZ == maxZ && ( Balls[i].pZ <= turns[0].pZ || Balls[i].pZ - 4*RADIUS * GlobalZSize <= turns[0].pZ))))
                return;
            if(!indX && indL && (Balls[i].pZ >= beginPoint.pZ
               || (beginPoint.pX == minX && (Balls[i].pX >= turns[0].pX || Balls[i].pX + 4*RADIUS * GlobalXSize >= turns[0].pX))
               || (beginPoint.pX == maxX && (Balls[i].pX <= turns[0].pX || Balls[i].pX - 4*RADIUS * GlobalXSize <= turns[0].pX))))
                return;
            if(!indX && !indL && ( Balls[i].pZ <= beginPoint.pZ
               || (beginPoint.pX == minX && (Balls[i].pX >= turns[0].pX || Balls[i].pX + 4*RADIUS * GlobalXSize >= turns[0].pX))
               || (beginPoint.pX == maxX && (Balls[i].pX <= turns[0].pX || Balls[i].pX - 4*RADIUS * GlobalXSize <= turns[0].pX))))
                return;
        }*/
/* TODO : BODOVI
        float min1X = minX , min1Z = minZ , max1X = maxX , max1Z = maxZ;
        if(beginPoint.pX == min1X || (indX && indL)) {
            maxX = turns[0].pX;
        }
        if(beginPoint.pX == max1X || (indX && !indL))
            minX = turns[0].pX;
        if(beginPoint.pZ == min1Z || (!indX && indL))
            maxZ = turns[0].pZ;
        if(beginPoint.pZ == max1Z || (!indX && !indL))
            minZ = turns[0].pZ;
*/
/*
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

}*/
