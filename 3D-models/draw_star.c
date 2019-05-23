#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

/* Dobra stara matematicka konstanta PI i jedna jako mala vrednost EPSILON.*/
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
    GLfloat ambient_coeffs[] = { 0.8, 0.8, 0.8, 1 };
    GLfloat diffuse_coeffs[] = { .7, .7, .7, 1 };
    //GLfloat specular_coeffs[] = { 1, 1, 1, 1 };
    GLfloat emission_coeffs[] = {1,1,220.0/256,0.25};
    //GLfloat shininess = 15;
    
    //glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    //glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
    glMaterialfv(GL_FRONT, GL_EMISSION, emission_coeffs);
    //glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}

void drawObject()
{
    setMaterial();
    
    glRotatef(-90,0,1,0);
    glutSolidCone(0.02,0.3,16,16);
    glRotatef(72,1,0,0);
    glutSolidCone(0.02,0.3,16,16);
    glRotatef(72,1,0,0);
    glutSolidCone(0.02,0.3,16,16);
    glRotatef(72,1,0,0);
    glutSolidCone(0.02,0.3,16,16);
    glRotatef(72,1,0,0);
    glutSolidCone(0.02,0.3,16,16);
    /*
     * Ovde treba implementirati
     * crtanje ciljnog objekta.
     */
}

static void on_display(void) /* Ciscenje prozora, podesavanje kamere i podesavanje svetla.*/
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(sin(angle)*2, 0, cos(angle)*2, 0, 0, 0, 0, 1, 0);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    /* Pozicija svetla (u pitanju je direkcionalno svetlo). */
    GLfloat light_position[] = { 1, 1, 1, 0 };

    /* Ambijentalna boja svetla. */
    GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1 };

    /* Difuzna boja svetla. */
    GLfloat light_diffuse[] = { 0.7, 0.7, 0.7, 1 };

    /* Spekularna boja svetla. */
    GLfloat light_specular[] = { 0.9, 0.9, 0.9, 1 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    
    glPushMatrix();
       drawObject(); /* Ovde primenjujemo drawObject() funkciju za iscrtavanje. */
    glPopMatrix();
    
    glutSwapBuffers();
}