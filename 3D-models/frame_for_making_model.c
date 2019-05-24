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
    /* Inicijalizuje se GLUT. */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    /* Kreira se prozor. */
    glutInitWindowSize(300, 300);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    /* Registruju se callback funkcije. */
    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);

    /* Obavlja se OpenGL inicijalizacija. */
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

void setMaterial()  /* Parametri materijala su stavljeni na default-ne vrednosti, pri konkretnoj primeni ih treba podesiti. */
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

void drawObject()
{
    setMaterial();
    /*
     * Ovde treba implementirati
     * crtanje ciljnog objekta.
     */
}

static void on_display(void) /* Ciscenje prozora, podesavanje kamere i podesavanje svetla.*/
{
    GLfloat light_position[] = { 1, 1, 1, 0 };
    GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1 };
    GLfloat light_diffuse[] = { 0.7, 0.7, 0.7, 1 };
    GLfloat light_specular[] = { 0.9, 0.9, 0.9, 1 };

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(sin(angle)*2, 0, cos(angle)*2, 0, 0, 0, 0, 1, 0);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    
    glPushMatrix();
       drawObject(); /* Ovde primenjujemo drawObject() funkciju za iscrtavanje. */
    glPopMatrix();
    
    glutSwapBuffers();
}
