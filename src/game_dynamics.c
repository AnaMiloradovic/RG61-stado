/* Izvorna datoteka sa implementacijama funkcija za dinamiku same igre. 
   Jedna od kljucnih izvornih datoteka za ovaj program. */
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
extern int NumOfSheeps, Level; /* Broj ovaca u igri i nivo igre. */
extern float minX,maxX,minZ,maxZ; /* Unutar ovih granica X i Z koordinata je slobodna povrsina za ovce. */
extern GLuint textureNames; /* Promenljiva u kojoj cuvamo generisano 'ime' teksture koju koristimo. */

int on_going = 1;     /* Glavna kontrolna promenljiva igre - da li je igra u toku ili moze da se tumaci kao da li se objketi krecu, animacija aktivna. */
int winner = 0;       /* Kontrolna promenljiva igre - da li je igrac pobedio. */
extern unsigned int pointsWon; /* Osvojeni poeni - srazmerni su pokrivenom procentu, ali se potezi sa jednim ili dva skretanja dodatno nagradjuju. */
extern float coveredPercent;  /* Pokriveni procenat terena. */
int hit = 0;          /* Jos jedna vazna kontrolna promenljiva igre - da li igrac trenutno iscrtava zatvarajucu putanju. */
char curDir = 0;      /* Trenutni pravac i smer kretanja zatvarajuceg objekata - azuriramo ga samo dok je hit = 1, jer nam je jedino tad bitan. */
                      /* Neophodan nam je kada detektujemo skretanje pri iscrtavanju zatvarajuce putanje. (vise informacija u funkciji drawHittingPath) */
int max_cur = 1;      /* Pomocni parametar za iscrtavanje zasipanja trave.
                         Promenjena da bude globalna, da bi mogle druge funkcije da joj pristupe i vrate je na 1 kada se zavrsi iscrtavanje putanje, bez toga pocetak njenog iscrtavanja ne bi izgledao uverljivo. */

/* Jedinice u svetskom koordinatnom sistemu. (vise informacija u fajlu draw_objects.c) */
extern double GlobalXSize;
extern double GlobalYSize;
extern double GlobalZSize;

void rollingBalls(int timer_id) /* Funkcija za kretanje ovcica, aktivna samo kada je igra u toku - glavna funkcija igre(iako se ne bi reklo po broju linija koda). */
{
    if(timer_id != TIMER_ID_ROLLING) /* Eventualna provera id timer-a . */
        return;

    int i;
    for(i=0;i<NumOfSheeps;i++)  /* Menjamo polozaj ovcica. */
    {
        Balls[i].pX += Balls[i].vX;
        Balls[i].pZ += Balls[i].vZ;
    }
    colissionDetection();      /*  Proveravamo da nije doslo do kolizije i u skladu sa tim menjamo smer brzine. */
    glutPostRedisplay();       /* Ponovno iscrtavanje. Funkciju pozivamo iz on_display funkcije, zato nije neophodno ovde ponovo registrovati timer. */
}

void colissionDetection()      /* Funkcija za detekciju kolizije medju ovcama i ovcama sa zidovima terena. */
{                              /* Ako dodje do kolizije, menja se smer kretanja ovcica. */
    int i,j,colission[]={TO_CHECK,TO_CHECK,TO_CHECK,TO_CHECK,TO_CHECK}; /* colission - pomocni niz koji nam govori da li smo vec brzinu odgovarajuce ovce. */
    for(i=0;i<NumOfSheeps;i++)                                          /* TO_CHECK - treba da se azurira. */
    {   /* Proveravamo prvo koliziju sa samim zidovima terena. */
        if(Balls[i].pX + ((float)RADIUS/MEADOWDIMENSION_X) >= maxX || Balls[i].pX  - ((float)RADIUS/MEADOWDIMENSION_X) <= minX) /* Ukoliko do kolizije dodje menjamo brzinu ovce, i stavljamo indikator da je brzina ovcice vec promenjena. */
        {
            Balls[i].vX = -Balls[i].vX;
            colission[i] = NOT_TO_CHECK;
        }
        if(Balls[i].pZ + ((float)RADIUS/MEADOWDIMENSION_Z) >= maxZ || Balls[i].pZ  - ((float)RADIUS/MEADOWDIMENSION_Z) <= minZ)
        {
            Balls[i].vZ = -Balls[i].vZ;
            colission[i] = NOT_TO_CHECK;
        }
    }
    
    for(i=0;i<NumOfSheeps;i++)  /* Proveravamo koliziju medju samim ovcicama. */
        for(j=i+1;j<NumOfSheeps;j++)
        {
            if(MEADOWDIMENSION_X*MEADOWDIMENSION_X*(Balls[i].pX-Balls[j].pX)*(Balls[i].pX-Balls[j].pX) +
                 MEADOWDIMENSION_Z*MEADOWDIMENSION_Z*(Balls[i].pZ-Balls[j].pZ)*(Balls[i].pZ-Balls[j].pZ) - RADIUS*RADIUS*2*2 < EPSILON)  
            {
                if(colission[i]) /* Ako joj je vec menjana brzina, ne moramo ovde to da radimo. */
                {
                   Balls[i].vX = -Balls[i].vX;
                   Balls[i].vZ = -Balls[i].vZ;
                   colission[i]= NOT_TO_CHECK;
                }
                if(colission[j])
                {
                   Balls[j].vX = -Balls[j].vX;
                   Balls[j].vZ = -Balls[j].vZ;
                   colission[j] = NOT_TO_CHECK;
                }
            }
        }
}

void drawHitting() /* Crtanje dogadjaja sa zatvarajucim objektom u toku iscrtavanja zatvarajuce putanje. */
{                 /*  Bez samog iscrtavanja putanje, samo pomocna crvena tackica na terenu na tekucoj poziciji i efekat zasipanja travom (kao metkici). */
    int i;
    const static int max_t = 10;  /* Maksimalan broj travcica - metkica koje se mogu iscrtati u bilo kom pozivu. */
    static int even_odd = 0; /* Jos jedan parametar za zasipanje trave - zavisi od poziva funkcije. Zahvaljujuci njemu imamo efekat kretanja travcica */
    const float step = 1;  /* Pomocna konstanta za izracunavanje rastojanja izmedju dva metkica. */

    glDisable(GL_LIGHTING); /* Crtanje pomocne crvene tackice. */
    glDisable(GL_LIGHT0);
    glColor3f(1, 0, 0);
    glPointSize(3);
    glBegin(GL_POINTS);
       glVertex3f(Closer.pX, 1.1, Closer.pZ);
    glEnd();
    
    glColor3f(0, 0.78, 0);  /* Zasipanje trave. */
    glPointSize(2.5);
    glBegin(GL_POINTS);
       for (i = 0; i < max_cur; i++)
          glVertex3f(Closer.pX - 0.15, Closer.pY - even_odd * 0.5 * step - i * step - 1.3, Closer.pZ);
    glEnd();

    if (max_cur < max_t) /* Azuriranje parametara za taj efekat za sledeci poziv. */
        max_cur++;
    if (even_odd)
        even_odd = 0;
    else even_odd++;

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
}

void drawHittingPath() /* Omotac funkcija za iscrtavanje zatvarajuce putanje - pre samog crtanja putanje vrsi neophodne provere. */
{
    static int firstCall = 1; /* Indikator da li je tek pocelo iscrtavanje zatvarajuce putanje. */
    static POINT turns[4];    /* Prva tacka na putanji + tacke skretanja. */
    static int numberOfTurns = 0; /* Ukupan broj tacaka skretanja + prva tacka na putanji. */
    static char currentDir = 0;  /* Dosadasnji pravac i smer kretanja - za detekciju skretanja. */
    if(firstCall)  /* Ako je tek pocelo iscrtavanje, proverava se da li je krenuo od ivice i inicijalizuju se parametri. */
    {
       if( Closer.pX != minX &&
           Closer.pX != maxX &&
           Closer.pZ != minZ &&
           Closer.pZ != maxZ)
        {
            hit = 0;
            return;
        }
        turns[0].pX = Closer.pX; /* Belezimo prvu tacku na nasoj putanji. */
        turns[0].pY = 1.1;
        turns[0].pZ = Closer.pZ;

        numberOfTurns = 1;      /* Azuriramo da imamo 1 relevantan element u nizu turns - prva tacka. */

        currentDir = curDir;    /* Belezimo tekuci pravac i smer kretanja zatvrajuces objekta. */
        firstCall = 0;          /* Menjamo indikator da se ne bi vise tokom crtanja trenutne putanje ulazilo u ovu granu. */
       return;
    }

    else if (currentDir != 0 && curDir != currentDir) /* Proveravamo da li je doslo do promene pravca i/ili smera. */
    {
        if((curDir == 'u' && currentDir == 'd') ||    /* Ako pokusavamo da idemo u suprotnom smeru, ponistava se iscrtavanje. */
           (curDir == 'd' && currentDir == 'u') ||
           (curDir == 'l' && currentDir == 'r') ||
           (curDir == 'r' && currentDir == 'l'))
        {
            firstCall = 1;
            curDir = 0;
            currentDir = 0;
            numberOfTurns = 0;
            hit = 0;
            max_cur = 1;
        }
        if(numberOfTurns == 4)  /* Ako je doslo do previse skretanja, takodje ponistavamo iscrtavanje. */
        {
            firstCall = 1;
            curDir = 0;
            currentDir = 0;
            numberOfTurns = 0;
            hit = 0;
            max_cur = 1;
        }
        turns[numberOfTurns].pX = Closer.pX; /* Inace, belezimo tacku na kojoj smo skrenuli. */
        turns[numberOfTurns].pY = 1.1;
        turns[numberOfTurns].pZ = Closer.pZ;

        switch(curDir) /* Zato sto smo se vec pomerili po jednoj koordinati od tacke,  */
        {             /* jedna trenutna koordinata nije validna, zato moramo u zavisnosti od pravca i smera kretanja da korigujemo vrednost. */
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
        numberOfTurns++;   /* Azuriramo broj validnih tacaka u nizu turns i azuriramo tekuci pravac i smer. */

        currentDir = curDir;
    }

    if(stepTest(turns,numberOfTurns)) /* Proveravamo da nam ovca nije nagazila na putanju. Ako jeste, ponistavamo iscrtavanje. */
    {
        firstCall = 1;
        curDir = 0;
        currentDir = 0;
        numberOfTurns = 0;
        hit = 0;
        max_cur = 1;
    }

    if(currentDir != 0 && ( Closer.pX >= maxX ||  /* Ako smo vec dosli na kraj iscrtavanja putanje, proveravamo da li su zatvorene nove povrsine. */
           Closer.pX <= minX ||
           Closer.pZ >= maxZ ||
           Closer.pZ <= minZ))
    {
        checkForNewPoints(turns,numberOfTurns);
        firstCall = 1;
        curDir = 0;
        currentDir = 0;
        numberOfTurns = 0;
        hit = 0;
        max_cur = 1;
    }

    if(currentDir == 0)
        currentDir = curDir;
    justDrawHittingPath(turns,numberOfTurns); /* Funkcija koja bas iscrtava putanju, definicija ispod. Saljemo joj tacke skretanja. */

}

void justDrawHittingPath(POINT* turns, int numberOfTurns)
{
    glDisable(GL_LIGHTING); /* Podesavamo boju, da bude linija isprekidana i u skladu sa tackama skretanja crtamo izlomljenu liniju. */
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

int stepTest(POINT* turns, int length) /* Test funkcija koja proverava da li je ovcica nagazila na nasu putanju. */
{
    POINT closerPosition;
    closerPosition.pX = Closer.pX;
    closerPosition.pY = 1.1;
    closerPosition.pZ = Closer.pZ;
    int i;
    for(i=0;i<NumOfSheeps;i++)
    {
        if(fabs((double) Closer.pX - Balls[i].pX) <  RADIUS*GlobalXSize && fabs((double) Closer.pZ - Balls[i].pZ) < RADIUS*GlobalZSize)
        {   /* Prvo provera da nismo kosom presli preko i-te ovce, ako jesmo, napustamo igru. */
            printf("Presli ste preko ovce\n\nKRAJ IGRE\n");
            glDeleteTextures(1,&textureNames);
            exit(EXIT_SUCCESS);
        }
       int j;
       for(j=0;j<length;j++) { /* Ovde proveravamo da i-ta ovca nije pregazila nasu putanju. */
           if (checkBetween(turns[j], turns[j + 1], Balls[i]))
               return 1;
       }
           if(checkBetween(turns[length-1],closerPosition,Balls[i]))
               return 1;
    }
    return 0;
}

int checkBetween(POINT p1, POINT p2, BALL pb) /* Pomocna funkcija za step - test, da li se ovca pb nalazi na duzi p1p2. */
{
    if(p1.pX == p2.pX && fabs((double)p1.pX - pb.pX) < RADIUS*GlobalXSize){
        if((p1.pZ <= pb.pZ && p2.pZ >= pb.pZ) || (p1.pZ >= pb.pZ && p2.pZ <= pb.pZ))
            return 1;}
    else if(p1.pZ == p2.pZ && fabs((double)p1.pZ - pb.pZ) < RADIUS*GlobalZSize){
        if((p1.pX <= pb.pX && p2.pX >= pb.pX) || (p1.pX >= pb.pX && p2.pX <= pb.pX))
            return 1;}
    return 0;

}

void checkForNewPoints(POINT* turns, int numberOfTurns) /* Omotac funkcija za proveru da li ima novih zagradjenih povrsina. */
{
    switch(numberOfTurns) /* U zavisnosti od broja skretanja se poziva razlicita funkcija za proveru i analizu. */
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
    printf("Pokriveni procenat:  %.2lf%% \n",coveredPercent); /* Ispisujemo trenutno zatvoren procenat povrsine. */
    printf("Osvojeno poena: %u \n\n",pointsWon);

    if((Level == 1 || Level == 2) && coveredPercent > 100 - NumOfSheeps * PERCENT_PER_SHEEP) /* Provera da nije igrac vec pobedio. (u zavisnosti od nivoa)*/
    {
        printf("Pobeda! BRAVOOO!!!\n");
        winning();
    }
    else if((Level ==3 || Level==4) && coveredPercent > 100 - NumOfSheeps * 0.75 * PERCENT_PER_SHEEP)
    {
        printf("Pobeda! BRAVOOO!!!\n");
        winning();
    }
    else if((Level == 5) && coveredPercent > 100 - NumOfSheeps * 0.5 * PERCENT_PER_SHEEP)
    {
        printf("Pobeda! BRAVOOO!!! \nSVAKA CAST MAJSTORE! \n");
        winning();
    }
}

void checkForNewPointsOne(POINT* turns) /* Provera kada nema skretanja na zatvarajucoj putanji. */
{
    char check_option, side; /* Pomocni parametri: check_option koju koordinatu ovaca bi trebalo da proveravamo, side - sa koje strane putanje(ovde duzi) bi trebalo da budu. */
    if(turns[0].pX == Closer.pX)
        check_option = 'x';
    else check_option = 'z';
    if(check_option == 'x') /* Na osnovu koordinata prve ovce utvrdjujemo sa koje strane duzi bi trebalo da se nalaze ovce. */
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
    for(i=1;i<NumOfSheeps;i++) /* Proveravamo polozaj ostalih ovcica, ako jedna nije gde bi trebalo, izlazi se iz funkcije i nema novih bodova. */
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
    /* Dosli smo u funkciji do ovde, znaci ima novih pokrivenih povrsina i novih poena. */
    if(check_option == 'x' && side == 'l')
    {
        coveredPercent += 100*(((maxX - Closer.pX)*(maxZ - minZ)) / WHOLE_MEADOW_SURFACE);  /* Azuriramo procenat i bodove. */
        pointsWon += ((maxX - Closer.pX) / (maxX - minX)) * 1000;
        maxX = Closer.pX;  /* I na kraju, naravno, novu granicu slobodne povrsine za ovce. Slicno i za ostale slucajeve. */
    }
    else if(check_option == 'x' && side == 'u')
    {
        coveredPercent += 100*(((Closer.pX - minX) * (maxZ - minZ)) / WHOLE_MEADOW_SURFACE);
        pointsWon += ((Closer.pX - minX) / (maxX - minX)) * 1000;
        minX = Closer.pX;
    }
    else if(check_option == 'z' && side == 'l')
    {
        coveredPercent += 100*(((maxZ - Closer.pZ)*(maxX- minX)) / WHOLE_MEADOW_SURFACE);
        pointsWon += ((maxZ - Closer.pZ) / (maxZ - minZ)) * 1000;
        maxZ = Closer.pZ;
    }
    else if(check_option == 'z' && side == 'u')
    {
        coveredPercent += 100*(((Closer.pZ -  minZ)*(maxX- minX)) / WHOLE_MEADOW_SURFACE);
        pointsWon += ((Closer.pZ - minZ) / (maxZ - minZ)) * 1000;
        minZ = Closer.pZ;
    }
}

void checkForNewPointsTwo(POINT* turns) /* Provera kada imamo jedno skretanje na nasoj putanji. */
{
     float check_x = turns[1].pX, check_z = turns[1].pZ; /* X i Z koordinate koje koristimo u proveri. Iako su vec zabelezene u turns[1], ovo radimo zbog citljivosti. */
     char x_side, z_side; /* Sa koje strane bi trebalo da se nalaze ovcice. */
     if(turns[0].pX == minX)  /* Ovde te parametri ne zavise od ispitivanja prve ovcice kao u funkciji ...One, nego od same putanje. */
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
     for(i=0;i<NumOfSheeps;i++) /* Ovde proveravamo da li su sve ovcice tamo gde bi trebalo da budu. */
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
     
     /* Dosli smo do ovde, znaci ima novih poena. U zavisnosti od pomocnih parametara azuriramo bodove, procenat i nove granice. */
     /* Ovo je smeliji i pametniji potez, zato ga nagradjujemo sa vise poena. */
    if(x_side == 'l' && z_side == 'l')
    {
        coveredPercent += 100.0*(((maxX - turns[1].pX) * (maxZ - minZ) + (maxZ - turns[1].pZ) * (turns[1].pX - minX))/WHOLE_MEADOW_SURFACE);
        pointsWon += (((maxX - turns[1].pX) * (maxZ - minZ) + (maxZ - turns[1].pZ) * (turns[1].pX - minX))/WHOLE_MEADOW_SURFACE) * 1500; 
        maxX = check_x;
        maxZ = check_z;
    }
    if(x_side == 'l' && z_side == 'u')
    {
        coveredPercent += 100.0*(((maxX - turns[1].pX) * (maxZ - minZ) + (turns[1].pZ - minZ ) * (turns[1].pX - minX))/WHOLE_MEADOW_SURFACE);
        pointsWon += (((maxX - turns[1].pX) * (maxZ - minZ) + (turns[1].pZ - minZ ) * (turns[1].pX - minX))/WHOLE_MEADOW_SURFACE) * 1500;
        maxX = check_x;
        minZ = check_z;
    }
    if(x_side == 'u' && z_side == 'l')
    {
        coveredPercent += 100.0*(((turns[1].pX - minX) * (maxZ - minZ) + (maxX - turns[1].pX) * (maxZ - turns[1].pZ) )/WHOLE_MEADOW_SURFACE);
        pointsWon += (((turns[1].pX - minX) * (maxZ - minZ) + (maxX - turns[1].pX) * (maxZ - turns[1].pZ) )/WHOLE_MEADOW_SURFACE) * 1500;
        minX = check_x;
        maxZ = check_z;
    }
    if(x_side == 'u' && z_side == 'u')
    {
        coveredPercent += 100*(((turns[1].pX - minX)*(maxZ - minZ) + (maxX - turns[1].pX) * (turns[1].pZ - minZ))/WHOLE_MEADOW_SURFACE);
        pointsWon += (((turns[1].pX - minX)*(maxZ - minZ) + (maxX - turns[1].pX) * (turns[1].pZ - minZ))/WHOLE_MEADOW_SURFACE) * 1500;
        minX = check_x;
        minZ = check_z;
    }

}

void checkForNewPointsThree(POINT* turns) /* Provera u slucaju da smo na putanji imali dva skretanja. */
{
    if(turns[0].pX != Closer.pX && turns[0].pZ != Closer.pZ) /* Ako smo se vratili u istu tacku od koje smo poceli, nista nam se ne racuna. */
        return;
    char check_parameter, side, range; /* Pomocni parametri :
                                                               
    - check_parameter -- za koju koordinatu proveravamo samo jednu granicu
    - side --  da li bi vrednost te koordinate trebala da bude manja ili veca od te granice 
    - range -- pojasnicu na primeru, vezano je za proveru dve granice za drugu koordinatu */

    if(turns[0].pX == minX)
    {
        check_parameter = 'x';
        side = 'l';
        if(turns[1].pZ < turns[2].pZ) /* Za drugu koordinatu proveravamo dve vrednosti, i range ce da nam oznaci da li je koordinata prve tacke skretanja veca ili ne. */
            range = 'a';             /* 'a' - ascending - nije veca, 'd'- descending - veca. Sve vrednosti ove koordinate ovcica moraju da se nalaze izmedju ove dve vrednosti. */
        else range = 'd';            /* Slicna provera i za ostale slucajeve. */
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
    for(i=0;i<NumOfSheeps;i++)  /* Posto smo inicijalizovali vrednosti pomocnih parametara, sada proveravamo da li su koordinate svih ovcica u ovim granicama. */
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
    /* Dosli smo do ovde, znaci bice novih poena. */
    /* Poene i procente koji se dodaju u ovoj funkciji cemo racunati iz vise delova. */
    /* Ovo je najtezi i najsmeliji potez i njega zato visestruko nagradjujemo. */
    if(check_parameter == 'x')
    {
        if(side == 'l')
        {
            coveredPercent += 100.0 * (((maxX - turns[1].pX) * (maxZ - minZ)) / WHOLE_MEADOW_SURFACE );
            pointsWon += 3000 * (((maxX - turns[1].pX) * (maxZ - minZ)) / WHOLE_MEADOW_SURFACE );
            maxX = turns[1].pX;
        }
        else
        {
            coveredPercent += 100.0 * (((turns[1].pX - minX) * (maxZ - minZ)) / WHOLE_MEADOW_SURFACE );
            pointsWon += 3000 * (((turns[1].pX - minX) * (maxZ - minZ)) / WHOLE_MEADOW_SURFACE );
            minX = turns[1].pX;
        }
        if(range == 'a')
        {
            coveredPercent += 100.0 * (((maxZ - turns[2].pZ) * (maxX - minX) + (turns[1].pZ - minZ) * (maxX - minX)) / WHOLE_MEADOW_SURFACE);
            pointsWon += 3000 * (((maxZ - turns[2].pZ) * (maxX - minX) + (turns[1].pZ - minZ) * (maxX - minX)) / WHOLE_MEADOW_SURFACE);
            minZ = turns[1].pZ;
            maxZ = turns[2].pZ;
        }else{
            coveredPercent += 100.0 * (((maxZ - turns[1].pZ) * (maxX - minX) + (turns[2].pZ - minZ) * (maxX - minX)) / WHOLE_MEADOW_SURFACE);
            pointsWon += 3000 * (((maxZ - turns[1].pZ) * (maxX - minX) + (turns[2].pZ - minZ) * (maxX - minX)) / WHOLE_MEADOW_SURFACE);
            minZ = turns[2].pZ;
            maxZ = turns[1].pZ;
        }
    }else{
        if(side == 'l')
        {
            coveredPercent += 100.0 * (((maxZ - turns[1].pZ) * (maxX - minX)) / WHOLE_MEADOW_SURFACE );
            pointsWon += 3000 * (((maxZ - turns[1].pZ) * (maxX - minX)) / WHOLE_MEADOW_SURFACE );
            maxZ = turns[1].pZ;
        }
        else
        {
            coveredPercent += 100.0 * (((turns[1].pZ - minZ) * (maxX - minX)) / WHOLE_MEADOW_SURFACE );
            pointsWon += 3000 * (((turns[1].pX - minZ) * (maxX - minX)) / WHOLE_MEADOW_SURFACE );
            minZ = turns[1].pZ;
        }
        if(range == 'a')
        {
            coveredPercent += 100.0 * (((maxX - turns[2].pX) * (maxZ - minZ) + (turns[1].pX - minX) * (maxZ - minZ)) / WHOLE_MEADOW_SURFACE);
            pointsWon += 3000 * (((maxX - turns[2].pX) * (maxZ - minZ) + (turns[1].pX - minX) * (maxZ - minZ)) / WHOLE_MEADOW_SURFACE);
            minX = turns[1].pX;
            maxX = turns[2].pX;
        }else{
            coveredPercent += 100.0 * (((maxX - turns[1].pX) * (maxZ - minZ) + (turns[2].pX - minX) * (maxZ - minZ)) / WHOLE_MEADOW_SURFACE);
            pointsWon += 3000 * (((maxX - turns[1].pX) * (maxZ - minZ) + (turns[2].pX - minX) * (maxZ - minZ)) / WHOLE_MEADOW_SURFACE);
            minX = turns[2].pX;
            maxX = turns[1].pX;
        }
    }

}

void winning() /* Funkcija koja se poziva kada se dodje do pobede. Ona samo menja kontrolne parametre da bi igra presla u pobednicko stanje. */
{
    on_going = 0;
    winner = 1;
    glutPostRedisplay();
}
