#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "inits.h"         /* Neophodno jer su u njemu nalaze deklaracije struktura objekata u igri. */
#include "game_dynamics.h" 

/* Deklaracije neophodnih promenljivih iz drugih datoteka. */
extern BALL Balls[];  /* Niz struktura ovaca - samo naziv ostao, jer je igra na pocetku razvijana sa kuglama. */    
extern CLOSER Closer; /* Promenljiva - struktura zatvarajuceg objekta. (kose) */
extern int NumOfSheeps; /* Broj ovaca u igri. */
extern float minX,maxX,minZ,maxZ; /* Unutar ovih granica X i Z koordinata je slobodna povrsina za ovce. */
extern GLuint textureNames; /* Promenljiva u kojoj cuvamo generisano 'ime' teksture koju koristimo. */

int on_going = 1;     /* Glavna kontrolna promenljiva igre - da li je igra u toku ili moze da se tumaci kao da li se objketi krecu, animacija aktivna. */
int winner = 0;       /* Kontrolna promenljiva igre - da li je igrac pobedio. */
extern unsigned int pointsWon; /* Osvojeni poeni - srazmerni su pokrivenom procentu, ali se potezi sa jednim ili dva skretanja dodatno nagradjuju. */
extern float coveredPercent;  /* Pokriveni procenat terena. */
int hit = 0;          /* Jos jedna vazna kontrolna promenljiva igre - da li igrac trenutno iscrtava zatvarajucu putanju. */
char curDir = 0;      /* Trenutni pravac i smer kretanja zatvarajuceg objekata - azuriramo ga samo dok je hit = 1, jer nam je jedino tad bitan. */

/* Jedinice u svetskom koordinatnom sistemu. (vise informacija u fajlu draw_objects.c) */
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
    }
    colissionDetection();
    glutPostRedisplay();
}

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

void justDrawHittingPath(POINT* turns, int numberOfTurns)
{
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glColor3f(.4,0.78,.4);

    glLineWidth(1.5);
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(5, 0xf0f0);
    glBegin(GL_LINE_STRIP);
    int i;
    for(i=0;i<numberOfTurns;i++)
        glVertex3f(turns[i].pX,turns[i].pY,turns[i].pZ);
    glVertex3f(Closer.pX,1.2,Closer.pZ);
    glEnd();
    glDisable(GL_LINE_STIPPLE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

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
            glDeleteTextures(1,&textureNames);
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

int checkBetween(POINT p1, POINT p2, BALL pb)
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
    glutPostRedisplay();
}
