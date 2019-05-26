/* Pomocni kod koji iscrtava kosu koju koristimo u igrici. */
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

/* Matematicka konstanta PI i jedna jako mala vrednost EPSILON.*/
#define PI 3.1415926535
#define EPSILON 0.01

/* Za koliki ugao ce nasa kamera biti zarotirana
 * u odnosu na Y-osu(u smeru suprotnom od smera kazaljke na satu). */
float angle=-180;

/* Deklaracije callback funkcija. */
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);

void setMaterial();  /* Funkcija koja postavlja drveni deo materijala na objekat. */
void setLightGrayMaterial(); /* Funkcija koja postavlja metalni deo materijala na objekat. */
void drawObject();   /* Funkcija koja iscrtava ciljni objekat. */
int main(int argc, char **argv)
{
    /* Inicijalizacija GLUT-a */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    /* Prozor i njegova podesavanja. */
    glutInitWindowSize(300, 300);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Scythe");

    /* Registrujemo callback funkcije. */
    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);

    /* OpenGL inicijalizacija - zadajemo boju pozadine i ukljucujemo testiranje z-koordinata. */
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
    gluPerspective(60, (float) width / height, 1, 100);
}

void setMaterial()  /* Postavljamo materijal na drveni deo objekta. */
{
    GLfloat materialCloserAmbient[] = {0.3,0.1,0.1,1};
    GLfloat materialCloserDiffuse[] = {0.4,0.1,0.1,1};
    GLfloat materialCloserSpecular[] = {0.5,0.5,0.5,1};
    GLfloat shininess=10;
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,materialCloserAmbient);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,materialCloserDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,materialCloserSpecular);
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS, shininess);
}

void setLightGrayMaterial() /* Postavljamo materijal na metalni deo objekta. */
{
    GLfloat materialCloserAmbient[] = {0.2,0.2,0.2,1};
    GLfloat materialCloserDiffuse[] = {0.8,0.8,0.8,1};
    GLfloat materialCloserSpecular[] = {0.9,0.9,0.9,1};
    GLfloat shininess=10;
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,materialCloserAmbient);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,materialCloserDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,materialCloserSpecular);
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS, shininess);
}
void set_normal_and_vertex(float u, float v) /* Da podesavamo vektore normala tacaka kada budemo iscrtavali - parametrizovali cilindar. */
{
    glNormal3f(
            sin(v),
            0,
            cos(v)
            );
    glVertex3f(
            sin(v),
            u,
            cos(v)
            );
}

void drawObject()   /* Telo funkcije koja nam iscrtava objekat - kosu. */
{
    setMaterial();  /* Postavljamo materijal na drveni deo objekta - prvo crtamo njega. */
    
    float u, v;

    glPushMatrix();   /* Parametrizacija valjka koji ceo kompletno biti zarotiran i nakrivljen - dovoljno samo glRotatef, */
    glRotatef(-30,0,0,1);            /*    i promenili smo koordinatni sistem u odnosu na koji parametrizujemo. */
    glScalef(0.25,1.2,0.25);   /* Hocemo da nam valjak ipak duzi i uzi. */
    for (u = 0; u < PI; u += PI / 20) {       /* Parametrizacija valjka. */
        glBegin(GL_TRIANGLE_STRIP);
        for (v = 0; v <= PI*2 + EPSILON; v += PI / 20) {   
            set_normal_and_vertex(u, v);
            set_normal_and_vertex(u + PI / 20, v);
        }
        glEnd();
    }

    /* Sada iscrtavamo metalni deo kose. (tj donji metalni kraj valjka i donji ostar deo) */
    glScalef(1.2,0.1,1.2);    /* Donji kraj valjka ce da bude kratak, a malo siri od drvenog dela. */
    setLightGrayMaterial();  /* Postavljamo metalni materijal. */
    for (u = 0; u < PI; u += PI / 20) {  
        glBegin(GL_TRIANGLE_STRIP);
        for (v = 0; v <= PI*2 + EPSILON; v += PI / 20) {     /*  Parametrizacija valjka. */
            set_normal_and_vertex(u, v);
            set_normal_and_vertex(u + PI / 20, v);
        }
        glEnd();
    }

    /* Sada vrsimo dalje transformacije i iscrtavamo ostar deo kose (koji je zapravo skalirana kupa). */
    glScalef(1/(1.2*0.25),1/(0.1*1.2),1/(1.2*0.25));  /* Neutralisemo prethodna (samo) skaliranja. (inverzna transformacija) */
    glRotatef(30,0,0,1);    
    glutSolidCone(0.2,3,20,20);
    glPopMatrix();

    /* Jos neki detalji koji se mogu dodati, nisam zelela onaj iz projekta da menjam.
    glPushMatrix();
    glTranslatef(1.5,2.6,0.3);
    glPushMatrix();
       glRotatef(60,0,0,1);
       glScalef(0.1,0.8,0.1);
       glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
       glTranslatef(-0.4,0.1,-0.4);
       glRotatef(-30,0,0,1);
       glPushMatrix();
          glRotatef(60,1,0,0);
          glScalef(0.1,0.8,0.1);
          glutSolidCube(1);
       glPopMatrix();
       glPushMatrix();
          glTranslatef(0.8,0,0);
          glRotatef(60,1,0,0);
          glScalef(0.1,0.8,0.1);
          glutSolidCube(1);
       glPopMatrix();
    glPopMatrix();
    glTranslatef(-0.8,-1.5,0);
    glPushMatrix();
       glRotatef(60,0,0,1);
       glScalef(0.1,0.8,0.1);
       glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
       glTranslatef(-0.4,0.1,-0.4);
       glRotatef(-30,0,0,1);
       glPushMatrix();
          glRotatef(60,1,0,0);
          glScalef(0.1,0.8,0.1);
          glutSolidCube(1);
       glPopMatrix();
       glPushMatrix();
          glTranslatef(0.8,0,0);
          glRotatef(60,1,0,0);
          glScalef(0.1,0.8,0.1);
          glutSolidCube(1);
       glPopMatrix();
    glPopMatrix();
    glPopMatrix();*/
}



static void on_display(void) 
{
    /* Ciscenje prozora, podesavanje kamere i podesavanje svetla.*/
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(5*cos(angle), 5, 5*sin(angle), 0, 0, 0, 0, 1, 0);
    
    /* Parametri svetla na sceni - ovde je direkciono svetlo. */
    GLfloat light_position[] = { 1, 1, 1, 0 };
    GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1 };
    GLfloat light_diffuse[] = { 0.7, 0.7, 0.7, 1 };
    GLfloat light_specular[] = { 0.9, 0.9, 0.9, 1 };
    /* Ukljucuje se osvjetljenje i registruju parametri svetla. */
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    
    
    glPushMatrix();
       drawObject(); /* Iscrtavanje objekta - ovde kose. */
    glPopMatrix();
    
    /* Nova slika se salje na ekran. */
    glutSwapBuffers();
}
