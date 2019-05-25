/* Pomocni kod koji iscrtava ogradu, slican uz dodatnu adaptaciju koriscen u igrici. */
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

/* Matematicka konstanta PI i jedna jako mala vrednost EPSILON.*/
#define PI 3.1415926535
#define EPSILON 0.01

/* Za koliki ugao ce nasa kamera biti zarotirana
 * u odnosu na Y-osu(u smeru suprotnom od smera kazaljke na satu). */
float angle=0;

/* Deklaracije callback funkcija. */
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);

void setMaterial();  /* Funkcija koja ce nam postaviti materijal na nas model.*/
void drawObject();   /* Funkcija koja iscrtava ciljni objekat. */
int main(int argc, char **argv)
{
   /* Inicijalizacija GLUT-a. */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    /* Prozor i njegova podesavanja. */
    glutInitWindowSize(300, 300);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Hedge");

    /* Registrujemo callback funkcije. */
    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);

    /* OpenGL inicijalizacija - zadajemo boju pozadine i ukljucujemo testiranje z- koordinata. */
    glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST);

    /* Ukljucujemo normalizaciju vektora normala */
    glEnable(GL_NORMALIZE);

    /* Program ulazi u glavnu petlju. */
    glutMainLoop();

    return 0;
}

static void on_keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 27:
        /* Zavrsava se program. */
        exit(0);
        break;
    case 'g':
        angle += PI/20;         /* Kada zelimo da pogledamo nas model iz druge tacke (npr da ga vidimo 'iz profila') */
        glutPostRedisplay();    /* na svaki stisak 'g' rotiramo se oko objekta za ugao od 9 stepeni u pozitivnom smeru.*/
        break;
    case 'h':
        angle -= PI/20;        /* Slicno kao i za 'g' samo se okrecemo za 9 stepeni u suprotnom smeru. */
        glutPostRedisplay();
        break;

    }
}

static void on_reshape(int width, int height)
{
    /* Podesava se viewport. */
    glViewport(0, 0, width, height);

    /* Podesava se projekcija. */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float) width / height, 1, 10);
}

void setMaterial()  /* Postavljamo drveni materijal. */
{
    GLfloat materialAmbient[] = {0.3,0.1,0.1,1};
    GLfloat materialDiffuse[] = {0.4,0.1,0.1,1};
    GLfloat materialForSquares[] = {0.5,0.5,0.5,1};
    GLfloat shininess=10;
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialForSquares);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}



void drawHedge(float minR, float maxR, int indX) /* Funkcija za iscrtavanje ograde, maxR - minR je duzina ograde, */
{                                                /* indX - indikator duz koje dimenzije zelimo da nam se iscrta ograda, indX - 0 - duz Z - ose, inace duz X - ose. */
    setMaterial();                               /* Delovi ograde su namerno pomalo nakrivljeni, da bi izgledali sto vernije. */
    glPushMatrix();
       if(indX) /* Leva (gornja) uspravna letva. */
       {
         glTranslatef(-(maxR-minR)*0.6,0,0);
         glRotatef(5,0,0,1);
       }else {
         glTranslatef(0,0,-(maxR-minR)*0.6);
         glRotatef(5,1,0,0);          
       }
       glScalef(0.3,1.5,0.3); 
       glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix(); /* Desna (donja) uspravna letva. */
       if(indX)
       {
          glTranslatef((maxR-minR)*0.6,0,0);
          glRotatef(2,0,0,1);
       } else{
          glTranslatef(0,0,(maxR-minR)*0.6);
          glRotatef(2,1,0,0);
       }
       glScalef(0.3,1.5,0.3);
       glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();  /* Gornja poprecna greda. */
       if(indX)
       {
          glTranslatef(0,0.2,0);
          glRotatef(4,0,0,1);
          glScalef(2*(maxR-minR),0.15,0.15);
       }
       else{
          glTranslatef(0,0.2,0);
          glRotatef(4,1,0,0);
          glScalef(0.15,0.15,2*(maxR-minR));
       }
       glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();  /* Donja poprecna greda. */
       if(indX)
       {
          glTranslatef(0,-0.25,0);
          glRotatef(2,0,0,1);
          glScalef(2*(maxR-minR),0.15,0.15);
       }else{
          glTranslatef(0,-0.25,0);
          glRotatef(2,1,0,0);
          glScalef(0.15,0.15,2*(maxR-minR));
       }
       glutSolidCube(1);
    glPopMatrix();
}

static void on_display(void)
{
    /* Parametri svetla na sceni - ovde je direkciono svetlo. */
    GLfloat light_position[] = { 1, 1, 1, 0 };
    GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1 };
    GLfloat light_diffuse[] = { 0.7, 0.7, 0.7, 1 };
    GLfloat light_specular[] = { 0.9, 0.9, 0.9, 1 };

    /* Cistimo prethodni sadrzaj prozora. */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    /* Podesava se vidna tacka. */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(sin(angle)*5, 0, cos(angle)*5, 0, 0, 0, 0, 1, 0);
    
    /* Ukljucuje se osvjetljenje i registruju parametri svetla. */
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    
    glPushMatrix();
       drawHedge(-0.5,0.5,1); /* Ovde primenjujemo funkciju za iscrtavanje. */
    glPopMatrix();
    
    glutSwapBuffers(); /* Nova slika se salje na ekran. */
}
