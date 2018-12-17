#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define RAZVOJNA_VERZIJA
//TODO: Kada budemo imali implementaciju iscrtavanja ovce i funkcionalnosti njenog kretanja,
//      sve neophodne funkcije i konstante ce biti deklarisane u ovom fajlu.
//#include "sheep.h"
#include "colors.h" //Ovde cuvamo boje koje koristimo u igri
#include "inits.h" //Ovde nam se nalaze deklaracije inicijalizacionih funkcija

#define TIMER_ID_0 0 //TODO: ID odgovarajuceg tajmera, neka za sad ovako ostane.
#define TIMER_INTERVAL 20 //TODO: Period ponovnog pozivanja tajmera
float pozX,pozZ; //HACK: Promenljive za pocetnu poziciju kugle.(kad uvedemo vise kugli, drugacije cemo
                       // cuvati informacije o pocetnim pozicijama kugli.
float angle = 0; //Ugao za koliki cemo da zarotiramo kuglu pri kotrljanju
float d_angle = 2; //Ugaona brzina kotrljanja
float vX=0.05,vZ=0.05; // Brzina kretanja kugle
char Name[]= "Stado - sacuvaj sve ovce! 🐑"; //Naslov prozora - i ime igre :)
int NumOfSheeps, Level;  //Ovde cuvamo broj ovaca, odnosno nivo igre
int width,height;

// NOTE: Za sada cemo ovde apstrahovati ovce belim kuglama koje se kotrljaju po
// terenu, pa cemo mozda u nekoj od poslednjih iteracija zameniti ovcama.
// NOTE: Ovde cemo za sada cuvati informacije o trenutnoj poziciji kugle na terenu
// U nekoj od narednih iteracija cemo takve informacije izmestiti u zasebnu datoteku.
// U ovoj verziji ce samo jedna brzina kretanja kugle biti implementirana.
float pX,pZ; // Trenutna pozicija kugle

void drawClouds();             //Crtanje oblaka
void onDisplayFunction();      // Funkcija za obradu dogadjaja
//TODO: static void onKeyboardFunction();  // Funkcija koja reaguje na dogadjaje sa tastature
void onReshapeFunction(int width, int height);   // Funkcija koja reaguje na promene velicine prozora
static void inline initialPos();  //Inicijalizuje prvobitan polozaj kugle
void rollingBalls(int timer_id); //U ovoj funkciji cemo implementirati kotrljanje kugli.//TODO: Dodaj zavisnost broja kugli od prosledjenog argumenta
int main(int argc, char* argv[])
{
   gameDataInitialization();
   
   glutInit(&argc,argv);  //Neophodna inicijalizacija glut-a
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

   graphicsInitialization();
   glutMainLoop();
   return 0;
}


void drawClouds()
{
    //glLineWidth(5);
    glDisable(GL_LIGHTING);
    glColor3f(0.97,0.97,0.97);   // Boja oblaka
    srand(time(NULL)); //Inicijalizujemo random-generator koji ce nam nasumicno generisati pozicije tackica u oblacicicma
    float cX,cY,cZ;  // Koordinate tacke u oblaku
    float cXMin, cXMax, // Granicni opsezi u okviru kojih cemo crtati oblake
            cYMin, cYMax,
            cZMin, cZMax;
    int  i,thickness; // Redom, pomocna indeksna promenljiva i gustina oblaka(ukupan broj tackica u oblaku)

    cXMin = -5, cXMax = 5;  // Prvi crtamo tako da moze da se proteze od zadnjeg desnog coska
    cYMin = 3.5, cYMax = 5;
    cZMin = 0, cZMax = 10;
    glBegin(GL_POINTS);
    thickness=2000;
    for(i=0;i<thickness;i++)
    {
        cX= ((float)rand()/RAND_MAX)*(cXMax-cXMin)+ cXMin;
        cY= ((float)rand()/RAND_MAX)*(cYMax-cYMin)+ cYMin;
        cZ= ((float)rand()/RAND_MAX)*(cZMax-cZMin)+ cZMin;

        glVertex3f(cX,cY,cZ);

    }


    cXMin = -5, cXMax = 0;
    cZMin = -10, cZMax = 0;


    for(i=0;i<thickness;i++)
    {
        cX= ((float)rand()/RAND_MAX)*(cXMax-cXMin)+ cXMin;
        cY= ((float)rand()/RAND_MAX)*(cYMax-cYMin)+ cYMin;
        cZ= ((float)rand()/RAND_MAX)*(cZMax-cZMin)+ cZMin;
        //glBegin(GL_POINTS);
        glVertex3f(cX,cY,cZ);
        //glEnd();
    }


    cXMin = 0, cXMax = 5;
    cZMin = -10, cZMax = 10;


    for(i=0;i<thickness;i++)
    {
        cX= ((float)rand()/RAND_MAX)*(cXMax-cXMin)+ cXMin;
        cY= ((float)rand()/RAND_MAX)*(cYMax-cYMin)+ cYMin;
        cZ= ((float)rand()/RAND_MAX)*(cZMax-cZMin)+ cZMin;
        //glBegin(GL_POINTS);
        glVertex3f(cX,cY,cZ);
        //glEnd();
    }
    glEnd();
    glLineWidth(1);
    glEnable(GL_LIGHTING);
}



void onDisplayFunction()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Cistimo sadrzaj prozora
    drawClouds();
    glEnable(GL_LIGHTING); //Postavljamo osvetljenje
    glEnable(GL_LIGHT0);

    // Pozicija i komponente osvetljenja
    GLfloat lightPosition[] = {-1,10,-1,0};
    GLfloat lightAmbient[]  = {1,1,1,1};
    GLfloat lightDiffuse[]  = {0.7,0.7,0.7,1};
    GLfloat lightSpecular[] = {1,1,1,1};

    // Opis materijala od kog je napravljena livada
    GLfloat materialGrassAmbient[]= {0,0.3,0,1};
    GLfloat materialGrassDiffuse[]= {0,0.4,0,1};
    GLfloat materialGrassSpecular[]= {0,1,0,1};
    GLfloat shininess =30 ;

    // Podesavanje osvetljenja
    glLightfv(GL_LIGHT0,GL_POSITION,lightPosition);
    glLightfv(GL_LIGHT0,GL_AMBIENT,lightAmbient);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,lightDiffuse);
    glLightfv(GL_LIGHT0,GL_SPECULAR,lightSpecular);

    // Postavljanje materijala na livadu
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,materialGrassAmbient);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,materialGrassDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,materialGrassSpecular);
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS, shininess);



    //glColor3f(0,1,0);
    glMatrixMode(GL_MODELVIEW);  //Podesavamo scenu i pogled
    glLoadIdentity();        // Cistimo matricu od prethodnog smeca.
    gluLookAt(0,4,13,0,0,0,0,1,0); // Pogled(posmatraceva pozicija, tacka pogleda, 'vektor nagore')



    #ifndef RAZVOJNA_VERZIJA
    //HACK: Moracu da ga popravim, da bi bilo lakse zbog kolizije sudara kugle sa ivicama zida
    //Iscrtavamo livadu(glavni teren) gde ce nam trcati ovce.
    glPushMatrix(); // Glavna ploca
    glScalef(10,0.58,20);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix(); //Sada crtamo ivice terena
    glScalef(1,0.5,1);

    glPushMatrix(); // Ivice po sirini terena (formalnije objasnjeno, duz x-ose)
    glScalef(10,1,0.5);
    glTranslatef(0,0.75,0); // Podizemo ivice tako da nam budu iznad terena
    //Crtamo sada svaku ponaosob od njih
    glPushMatrix();
    glTranslatef(0,0,19.5);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,0,-19.5);
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();

    glPushMatrix(); // Ivice duz terena (i duz z-ose)
    glScalef(0.5,1,20);
    glTranslatef(0,0.75,0); // Podizemo ivice tako da nam budu iznad terena
    //Crtamo sada svaku ponaosob od njih
    glPushMatrix();
    glTranslatef(9.5,0,0);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-9.5,0,0);
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();
    #endif

    //Iscrtavamo livadu(glavni teren) gde ce nam trcati ovce.
    glPushMatrix();
    glScalef(10,0.25,20);
    glutSolidCube(1);
    glPopMatrix();

    // Iscrtavamo ivice duz terena (i duz z-ose)
    glPushMatrix();
    glScalef(0.5,0.8,21);
    //Crtamo sada svaku ivicu ponaosob od njih
    glPushMatrix();
    glTranslatef(10.5,0.2,0);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-10.5,0.2,0);
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();
    // Ivice po sirini terena (formalnije objasnjeno, duz x-ose)
    glPushMatrix();
    glScalef(10,0.8,0.5);
    //Crtamo sada svaku ivicu ponaosob od njih
    glPushMatrix();
    glTranslatef(0,0.2,20.5);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,0.2,-20.5);
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();

    //Podesavamo odgovarajuce materijale za kugle
    GLfloat materialBallAmbient[] = {0.7,0.7,0.7,1};
    GLfloat materialBallDiffuse[] = {0.6,0.6,0.6,1};
    GLfloat materialBallSpecular[] = {0.75,0.8,0.75,1};
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,materialBallAmbient);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,materialBallDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,materialBallSpecular);
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS, shininess);
    //glLoadIdentity(); //Neka nam je za svaki slucaj tu

    //glScalef(1,0.25,1);
    //glTranslatef(0,2.5,0); //Dizemo kuglu na povrsinu terena
    //glScalef(1,4,1);
    glPushMatrix();
    initialPos();
    glTranslatef(pozX,0.625,pozZ);
    glRotatef(angle,0,0,1);
    glutSolidSphere(0.5,16,16);
    glPopMatrix();


    glutTimerFunc(TIMER_INTERVAL,rollingBalls,TIMER_ID_0);

    /*glScalef(5,0.25,10); //Podesavamo koordinatni sistem tako da nam bude vezan za sam teren
    initialPos();
    glTranslatef(pozX,0,pozZ);
    glRotatef(angle,0,0,1);
    glutTimerFunc(TIMER_INTERVAL,rollingBalls,TIMER_ID_0);*/
    glutSwapBuffers();
}

void onReshapeFunction(int w, int h)
{
    width=w;
    height=h;
    glViewport(0,0,width,height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();             //Cistimo matricu od prethodnog smeca.
    gluPerspective(90,(float) width/height,1,40); //Podesavamo projekciju
}
    

void gameDataInitialization()
{
   int toContinue=0; //Indikator korektnog unosa
   printf("Izaberite nivo(1-10) i broj ovaca(1-5):\n\n-> ");
   while(!toContinue)  //Dok se ne unesu korektni podaci
   {
      scanf("%d%d",&Level,&NumOfSheeps);
      if(Level < 1 || Level > 10 || NumOfSheeps < 1 || NumOfSheeps > 5)
          printf("Niste korektno uneli nivo i broj ovaca:\n-> ");
      else toContinue=1; //Podaci su korektno uneti, mozemo iz petlje
   }

}

void graphicsInitialization()
{
    glutInitWindowSize(WINDOW_WIDTH,WINDOW_HEIGHT); //Kreiranje prozora i neophodne inicijalizacije
    glutInitWindowPosition(WINDOW_POSITION_X,WINDOW_POSITION_Y);
    glutCreateWindow(Name);
    glutDisplayFunc(onDisplayFunction);  //Registrujemo callback funkcije
    glutReshapeFunc(onReshapeFunction);
    glClearColor(COLOR_SKY_R,COLOR_SKY_G,COLOR_SKY_B,0);  //Boja neba - pre nego sto postavimo osvetljenje
    glEnable(GL_DEPTH_TEST);
    return;
}

static void inline initialPos()
{
    static int broj_pozivanja=0; //Telo funkcije izvrsavamo SAMO JEDNOM, i to na pocetku kada random inicijalizujemo
                                // pocetnu poziciju kugle. Ova staticka promenljiva lokalnog dosega nam cuva informaciju da li
                                // prvi put pozivamo ovu funkciju.
    if(broj_pozivanja)
        return;
    srand(time(NULL)); //Inicijalizujemo nas random generator
    pozX = ((float)rand()/RAND_MAX)*(10-1) - 5 ;  //Inicijalizujemo pocetnu poziciju (za sad jedne) kugle
    pozZ = ((float)rand()/RAND_MAX)*(20-1)  - 10;
    broj_pozivanja++; //Posle ovoga vise nikad necemo izvrsavati naredbe iznad.
}


void rollingBalls(int timer_id)
{
    if(timer_id != TIMER_ID_0)
        return;
    pozX += vX;
    if(pozX + 0.5 > 5 || pozX  - 0.5 < -5) {

        vX = -vX;
        d_angle = -d_angle;
    }
    pozZ += vZ;
    if(pozZ + 1 > 10 || pozZ  - 1 < -10) {
        vZ = -vZ;
        d_angle = -d_angle;
    }
    angle += d_angle;
    if(angle > 360)
        angle -= 360;
    if(angle < 0)
        angle += 360;
    glutPostRedisplay();

}