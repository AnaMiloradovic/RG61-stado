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
/* **************************************** */
int hit = 0;
int hitC = 0;
int changeD = 0;
char curDir = 0;


/* **************************************** */
unsigned char moving;
/* HACK: extern SURFACE ClosedSurfaces[MAX_POSSIBLE_CLOSED_SURFACES];
   HACK: extern int NumOfClosedSurfaces;

   HACK: extern float PercentOfCoveredField;  */
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
    static POINT beginPoint; /* Pocetna tacka putanje */
    static POINT turns[3]; /* Maks 3 skretanja */
    static int  numberOfTurns = 0,indBegin = 1;
    int i;
    if(indBegin)
    {
        indBegin = 0;
        hitC = 1;
        beginPoint.pX = Closer.pX;
        beginPoint.pY =1.01;
        beginPoint.pZ = Closer.pZ;
        return;
    }
    if(changeD)
    {
        changeD = 0;
        turns[numberOfTurns].pX = Closer.pX;
        turns[numberOfTurns].pY = 1.01;
        turns[numberOfTurns].pZ = Closer.pZ;
        numberOfTurns++;
    }

    glDisable(GL_LIGHTING);
    glColor3f(1,0,0);
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(5, 0xf0f0);
    glBegin(GL_LINE_STRIP);
    glVertex3fv(&beginPoint);
    for(i=0;i<numberOfTurns;i++)
       glVertex3fv(turns+i);
    glVertex3f(Closer.pX,1.01,Closer.pZ);
    glEnd();
    glDisable(GL_LINE_STIPPLE);
    glEnable(GL_LIGHTING);
}
/* HACK
void tryToClose()
{
    static POINT turningPoints[MAX];
    static int numOfTurningPoints = 0;
    static int isBegin=1;
    static unsigned char currentMoving = ' ';
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);
    glColor3f(1,0,0);
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex3f(Closer.pX,Closer.pY,Closer.pZ);
    glVertex3f(Closer.pX,1,Closer.pZ);
    glEnd();
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    if(!closing)
    {
        closing=1;
        turningPoints[0].pX = Closer.pX;
        turningPoints[0].pZ = Closer.pZ;
        printf("ISCRTAVANJE: \nPrva tacka:    X: %f , Z: %f\n\n",Closer.pX,Closer.pZ);
        currentMoving = ' ';
        return;
    }
    
    if(currentMoving == ' ')
    {
        currentMoving = moving;
        return;
    }

    if(isBegin && ( ( (turningPoints[0].pX == minX || turningPoints[0].pX == maxX) && fabs(Closer.pX - turningPoints[0].pX) > EPSILON) ||
            ( (turningPoints[0].pZ == minZ || turningPoints[0].pZ == maxZ) && fabs(Closer.pZ - turningPoints[0].pZ) > EPSILON) ) )
            isBegin = 0;
    
    if(moving != currentMoving)
    {
        numOfTurningPoints++;
        if(numOfTurningPoints >= MAX)
        {
            printf("KRAJ ISCRTAVANJA: previse prelomnih tacaka, neuspesno, neuspesno\n");
            isBegin = 1;
            numOfTurningPoints=0;
            currentMoving = ' ';
            closing = 0;
            return;
        }
        if(abs(currentMoving - moving) == 2)
        {
            printf("KRAJ ISCRTAVANJA: suprotan smer, neuspesno\n");
            isBegin = 1;
           numOfTurningPoints=0;
           currentMoving = ' ';
           closing = 0;
           return;
        }
        currentMoving = moving;
        turningPoints[numOfTurningPoints].pX = Closer.pX;
        turningPoints[numOfTurningPoints].pZ = Closer.pZ;
        switch(moving)
        {
            case 'i':
                turningPoints[numOfTurningPoints].pZ = Closer.pZ + Closer.v;
                break;
            case 'j':
                turningPoints[numOfTurningPoints].pX = Closer.pX + Closer.v;
                break;
            case 'k':
                turningPoints[numOfTurningPoints].pZ = Closer.pZ - Closer.v;
                break;
            case 'l':
                turningPoints[numOfTurningPoints].pX = Closer.pX - Closer.v;
                break;
        }
        printf("Prelomna tacka %d :   X: %f , Z: %f\n",numOfTurningPoints,turningPoints[numOfTurningPoints].pX,turningPoints[numOfTurningPoints].pZ);
        printf("Valjak:   X: %f , Z: %f \n\n",Closer.pX,Closer.pZ);
    }
    
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);
    
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1,0xfff0);
    glLineWidth(2);
    glColor3f(1,0,0);
    
    glBegin(GL_LINE_STRIP);
       for(int i=0;i<=numOfTurningPoints;i++)
           glVertex3f(turningPoints[i].pX,1.1,turningPoints[i].pZ);
       glVertex3f(Closer.pX,1.1,Closer.pZ);
    glEnd();
    glDisable(GL_LINE_STIPPLE);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    
    
    for(int i=0;i<NumOfSheeps;i++)
    {
        for(int j=0;j<numOfTurningPoints;j++)
        {
            if(turningPoints[j].pX == turningPoints[j+1].pX)
            {
                if(fabs(Balls[i].pX - turningPoints[j].pX) < EPSILON && 
                    ((Balls[i].pZ <= turningPoints[j+1].pZ + EPSILON && Balls[i].pZ >= turningPoints[j].pZ - EPSILON) ||
                     (Balls[i].pZ >= turningPoints[j+1].pZ - EPSILON && Balls[i].pZ <= turningPoints[j].pZ + EPSILON)))
                    {
                        printf("KRAJ ISCRTAVANJA, nagazila ovca, neuspesno!\n");
                        isBegin = 1;
                        closing =0;
                        numOfTurningPoints = 0;
                        currentMoving = ' ';
                        return;
                    }
            }
            else if(turningPoints[j].pZ == turningPoints[j+1].pZ)
            {
                if(fabs(Balls[i].pZ - turningPoints[j].pZ) < EPSILON && 
                    ((Balls[i].pX <= turningPoints[j+1].pX + EPSILON && Balls[i].pX >= turningPoints[j].pX - EPSILON) ||
                     (Balls[i].pX >= turningPoints[j+1].pX - EPSILON && Balls[i].pX <= turningPoints[j].pX + EPSILON)))
                    {
                        printf("KRAJ ISCRTAVANJA, nagazila ovca, neuspesno!\n");
                        isBegin = 1;
                        closing =0;
                        numOfTurningPoints = 0;
                        currentMoving = ' ';
                        return;
                    }
            }
        }
        if(fabs(Closer.pX-Balls[i].pX) < 0.25*RADIUS/MEADOWDIMENSION_X && fabs(Closer.pZ -Balls[i].pZ) < 0.5*RADIUS/MEADOWDIMENSION_Z)
        {
            printf("Game over\nIzgubili ste ovcu!\n");
            exit(0);
        }
        if(turningPoints[numOfTurningPoints].pX == Closer.pX)
        {
                if(fabs(Balls[i].pX - turningPoints[numOfTurningPoints].pX) < EPSILON && 
                    ((Balls[i].pZ <= turningPoints[numOfTurningPoints].pZ + EPSILON && Balls[i].pZ >= Closer.pZ - EPSILON) ||
                     (Balls[i].pZ >= turningPoints[numOfTurningPoints].pZ - EPSILON && Balls[i].pZ <= Closer.pZ + EPSILON)))
                    {
                        printf("KRAJ ISCRTAVANJA, nagazila ovca, neuspesno!\n");
                        isBegin = 1;
                        closing =0;
                        numOfTurningPoints = 0;
                        currentMoving = ' ';
                        return;
                    }
        }
        else if(turningPoints[numOfTurningPoints].pZ == Closer.pZ)
        {
                if(fabs(Balls[i].pZ - turningPoints[numOfTurningPoints].pZ) < EPSILON && 
                    ((Balls[i].pX <= turningPoints[numOfTurningPoints].pX + EPSILON && Balls[i].pX >= Closer.pX - EPSILON) ||
                     (Balls[i].pX >= turningPoints[numOfTurningPoints].pX - EPSILON && Balls[i].pX <= Closer.pX + EPSILON)))
                    {
                        printf("KRAJ ISCRTAVANJA, nagazila ovca, neuspesno!\n");
                        isBegin = 1;
                        closing =0;
                        numOfTurningPoints = 0;
                        currentMoving = ' ';
                        return;
                    }
        }
    }
    
    if(!isBegin  && ((numOfTurningPoints == 3 && turningPoints[0].pX == Closer.pX && turningPoints[0].pZ == Closer.pZ ) || fabs(Closer.pX - maxX) < EPSILON || fabs(Closer.pX - minX) < EPSILON
        || fabs(Closer.pZ - maxZ) < EPSILON || fabs(Closer.pZ - minZ) < EPSILON) )
    {
        checkToClosedSurface(turningPoints,numOfTurningPoints);
        printf("KRAJ ISCRTAVANJA, USPESNO!\n");
        isBegin = 1;
        numOfTurningPoints=0;
        currentMoving = ' ';
        closing = 0;
        return;
    }
}
*/

/* HACK:
void checkToClosedSurface(POINT turningPoints[],int numOfTurningPoints)
{
    float firstPointX = turningPoints[0].pX, firstPointZ = turningPoints[0].pZ; // Koordinate prve tacke na putanji
    if(numOfTurningPoints != 3 && firstPointX != minX && firstPointX!= maxX &&
           firstPointZ != minZ && firstPointZ != maxZ) // Iskljucujemo sve putanje koje ne pocinju od neke ivice,
        // osim ako broj prelomnih tacaka nije 3(jer onda postoji mogucnost da je igrac bio dovoljno vest i sposoban
        // da opise netaknuti pravougaonik oko svih ovaca
        return;
    // Dalje, sada ispitujemo sa koje ivice krece putanja, i u zavisnosti
    if(numOfTurningPoints == 0) //Prvo ispitujemo da nije obicna pravolinijska putanja koja samo treba da dodje do drugog kraja terena
    {
        if( (firstPointX == minX && fabs(Closer.pX - maxX) < EPSILON) ||
                (firstPointX == maxX && fabs(Closer.pX - minX) < EPSILON) )
        {// Valjak je uspesno presao sa levog na desni kraj terena. Sada treba da vidimo da li je pri tom eventualno zatvoren gornji
            // ili donji deo terena (tj. da bi se neki od njih uspesno zatvorio, na njemu u trenutku kraja iscrtavanja putanje ne sme da bude nijedna ovca)
            int indicator = 1;
         for(int i=0;i<NumOfSheeps;i++)  //Proveravamo prvo donji deo terena, ispod nacrtane prave
         {
             if(Balls[i].pZ > firstPointZ)
             {
                 indicator = 0; //Ovca je, dakle, u donjem delu terena, znaci ne mozemo ga zatvoriti
                 break;
             }
         }
         if(indicator) // Nije do sad negiran indikator, znaci da dole nema ovaca, znaci donji deo terena zatvaramo
         {
             // Dodajemo novi zatvoreni pravougaonik u nas niz, azuriramo zatvoreni procenat terena, i na kraju azuriramo
             // informaciju o maksimalnoj z- koordinati koja je ovcama i dalje slobodna
             printf("Prekrivena povrsina");
             ClosedSurfaces[NumOfClosedSurfaces++] = (SURFACE) {minX, firstPointZ, maxX, maxZ};

             PercentOfCoveredField += ((maxZ - firstPointZ)*(maxX - minX)/4)*100;  //4 je ukupna povrsina terena(koji je ovde skaliran na dimenzije 2x2)
             printf("  Procenat: %f \n",PercentOfCoveredField);
             maxZ = firstPointZ;
             return; //Obradili smo zatvaranje, mozemo van
         }
         else //Ok, sada proveravamo gornji deo terena. Ako na njemu nema ovaca, dalja obrada je analogna, samo sto sada radimo sa minimalnom z koordinatom.
             //Provera je neophodna, jer se opet moze dogoditi da je igrac uspesno iscrtao pravu liniju, ali sa obe strane se nalaze ovce,
             // tada ne moze nista zagraditi.
         {
             for(int i=0;i<NumOfSheeps;i++)
                 if(Balls[i].pZ < firstPointZ)
                     return; //Mozemo u slucaju da se bar jedna ovca nalazi sa gornje strane da odmah izadjemo iz funkcije, posto je donja strana vec ispitana
             // Ako smo nakon petlje dosli do ove tacke, to znaci da sa gornje strane nema nijedne ovce, i da nju treba zagraditi.
             printf("Prekrivena povrsina");
             ClosedSurfaces[NumOfClosedSurfaces++] = (SURFACE) {minX, minZ, maxX, firstPointZ};

             PercentOfCoveredField += ((firstPointZ - minZ)*(maxX - minX)/4)*100;
             printf("  Procenat: %f \n",PercentOfCoveredField);
             minZ = firstPointZ;
             return;
         }
        }
        // Analogno sada sve obrade i ispitivanja vrsimo kada je valjak presao uzduz terena
        else
        {
            int indicator = 1;
            for(int i=0;i<NumOfSheeps;i++)  //Proveravamo desni deo terna
            {
                if(Balls[i].pX > firstPointX)
                {
                    indicator = 0;
                    break;
                }
            }
            if(indicator) // Mozemo da zatvorimo desni deo terena
            {
                ClosedSurfaces[NumOfClosedSurfaces++] = (SURFACE) {firstPointX, minZ, maxX, maxZ};

                PercentOfCoveredField += ((maxZ - minZ)*(maxX - firstPointX)/4.0)*100;
                maxX = firstPointX;
                printf("Prekrivena povrsina");
                printf("  Procenat: %f \n",PercentOfCoveredField);
                return; //Obradili smo zatvaranje, mozemo van
            }
            else // Proveravamo mozemo li zatvoriti levi deo terena
            {
                for(int i=0;i<NumOfSheeps;i++)
                    if(Balls[i].pX < firstPointX)
                        return; //Mozemo u slucaju da se bar jedna ovca nalazi sa leve strane da odmah izadjemo
                // Ako smo uspeli da dodjemo do ovde, zagradjujemo levu stranu
                printf("Prekrivena povrsina");
                ClosedSurfaces[NumOfClosedSurfaces++] = (SURFACE) {minX, minZ, firstPointX, maxZ};

                PercentOfCoveredField += ((maxZ - minZ)*(firstPointX - minX)/4)*100;
                printf("  Procenat: %f \n",PercentOfCoveredField);
                minX = firstPointX;
                return;
            }
        }
    }
    else if(numOfTurningPoints == 1)
    {
        // Ponovo cemo ispitivati sa koje ivice je pocetna tacka
        if(firstPointZ == minZ)
        {// Dakle, prvi deo izlomljene putanje je duz z-ose, od vrha ka dole. Treba prvo da odredimo da li se eventualno
        // sve ovce nalaze sa leve ili sa desne strane prvog segmenta putanje(zapravo i same pocetne tacke). Takodje, trebalo bi
        // da proverimo i da li se sve one nalaze (ovde) iznad drugog segmenta ove putanje (zapravo da li su njihove z-koordinate
        // manje od z-koordinate prelomne tacke, jer ako nisu svima, onda ih ova putanja sigurno ne moze pravilno zatvoriti. Kad i to ispitamo,
        // ostaje nam da proverimo da li se putanja zavrsava sa iste strane prvog segmenta sa koje su i tacke, ako da, putanja zatvara tacke.
           int indicator=1; //Prvo proveravamo da li su sve ovce sa leve strane prvog segmenta(zapravo, prve tacke ove putanje)
           for(int i=0;i<NumOfSheeps;i++)
               if(Balls[i].pX > firstPointX)
               {
                   indicator = 0;
                   break;
               }
           if(indicator && Closer.pX == maxX)  //Ako se sve ovce nalaze sa leve strane, a poslednja tacka putanje sa desne, onda ovo nije zatvarajuca putanja
               return;
           for(int i=0;i<NumOfSheeps;i++) //Sada proveravamo da nisu sve sa desne strane, ako se za bilo koju ispostavi da nije, tu mozemo da zavrsimo svako razmatranje
               if(Balls[i].pX < firstPointX)
                   return;
           if(Closer.pX == minX)
               return;
           //Sada nam jos ostaje da proverimo da li su sve tacke(ovde) iznad prelomne tacke
           for(int i=0;i<NumOfSheeps;i++)
               if(Balls[i].pZ < turningPoints[1].pZ)
                   return;
           //Ako smo dosli do ovde, imamo zatvarajucu putanju :) Jos sada da azuriramo povrsine i procenat :D

           if(indicator) //Sve su ovce sa leve strane
           {
               ClosedSurfaces[NumOfClosedSurfaces++] = (SURFACE) {firstPointX,minZ,maxX,turningPoints[1].pZ};
               PercentOfCoveredField += ((maxX-firstPointX)*(turningPoints[1].pZ- minZ)/4)*100;

               ClosedSurfaces[NumOfClosedSurfaces++] = (SURFACE) {turningPoints[1].pX,turningPoints[1].pZ,maxX,maxZ};
               PercentOfCoveredField += ((maxX-turningPoints[1].pX)*(maxZ-turningPoints[1].pZ)/4)*100;

               ClosedSurfaces[NumOfClosedSurfaces++] = (SURFACE) {minX, Closer.pZ, turningPoints[1].pX, maxZ};
               PercentOfCoveredField += ((turningPoints[1].pX-minX)*(maxZ-Closer.pZ)/4)*100;

               maxX = firstPointX;
               maxZ = turningPoints[1].pZ;
               return;
           }

        }
    }

}
 */