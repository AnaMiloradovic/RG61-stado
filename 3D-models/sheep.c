/* Pomocni kod koji iscrtava ovcu - glavnog lika u nasoj igrici. */
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

/* Matematicka konstanta PI, jedna jako mala vrednost EPSILON 
i R- poluprecnik loptica od kojih pravimo telo ovce.(vunu) */
#define PI 3.1415926535
#define EPSILON 0.01
#define R 0.075

/* Za koliki ugao ce nasa kamera biti zarotirana
 * u odnosu na Y-osu(u smeru suprotnom od smera kazaljke na satu). */
float angle=0;

/* Deklaracije callback funkcija. */
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);

void  addSphereCenterBody(float x, float y);  /* Dodavanje loptica koje ce biti u sredisnjem delu tela. */
void  addSphereBodySymetric(float x, float y, float z); /* Dodajemo bocno kuglicu i sa leve i simetricno sa desne strane tela. */
void draw_object();                          /* Funkcija u kojoj kompletno crtamo ovcu. */
void drawLeg(float lX,float lZ, float sX, float sZ); /* Crtanje noge. Parametri: X i Z koordinata noge, sX i sZ su ,redom, indikatori okrenutosti u odnosu na X-osu
                                                       (pozitivan ili negativan smer) i strana Z-ose sa koje se nalazi noga.*/
void drawHead(); /* Iscrtavanje kompletno glave. */
void drawBody(); /* Iscrtavanje vunenog tela. */
void setSkinMaterial();   /* Postavljanje materijala na glavu i noge, oci i vunu. */
void setEyeMaterial();
void setWoolMaterial();
int main(int argc, char **argv)
{
    /* Inicijalizacija GLUT-a. */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    /* Prozor i njegova podesavanja. */
    glutInitWindowSize(300, 300);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Sheep 🐑");

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
    case 'g':              /* Kada zelimo da pogledamo nas model iz druge tacke (npr da ga vidimo 'iz profila') */
        angle += PI/20;    /* na svaki stisak 'g' rotiramo se oko objekta za ugao od 9 stepeni u pozitivnom smeru.*/
        glutPostRedisplay();
        break;
    case 'h':
        angle -= PI/20;     /* Slicno kao i za 'g' samo se okrecemo za 9 stepeni u suprotnom smeru. */
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

void inline addSphereCenterBody(float x, float y) /* Dodavanje loptice vune na sredisnji deo tela. */
{
    glPushMatrix();
       glTranslatef(x,y,0);
       glutSolidSphere(0.3,16,16);
    glPopMatrix();
}

void inline addSphereBodySymetric(float x, float y, float z)  /* Bocno dodavanje kuglice simetricno sa obe strane tela.(simetricno u odnosu na x-osu u lokalnom koordinatnom sistemu.*/
{
    glPushMatrix();
       glTranslatef(x,y,0);
       glPushMatrix();
          glTranslatef(0,0,z);
          glutSolidSphere(0.3,16,16);
       glPopMatrix();
       glPushMatrix();
          glScalef(1,1,-1);
          glTranslatef(0,0,z);
          glutSolidSphere(0.3,16,16);
       glPopMatrix();
    glPopMatrix();
}

void drawLeg(float lX,float lZ, float backLeg, float oppositeSide) /* Iscrtavanje noge. */
{
     glPushMatrix();
        glScalef(1,1,oppositeSide);
        glTranslatef(lX,0,lZ);
        glRotatef(backLeg*20,-1,0,1);
        float u,h;
        glBegin(GL_TRIANGLE_STRIP);
        for(h=0;h>= -0.5;h-=PI/20)
           for(u=0;u<2*PI+EPSILON;u+=PI/20)
           {
               glNormal3f(cos(u),0,sin(u));
               glVertex3f(R*cos(u),h,R*sin(u));
               glVertex3f(R*cos(u),h-PI/20,R*sin(u));
           }
        glEnd();
        glTranslatef(0,-0.63,0);
        glutSolidSphere(R,16,16);
    glPopMatrix();
}

void drawHead()  /* Iscrtavanje glave. */
{
    glTranslatef(0.4,0.2,0.27);
    glScalef(1.1,1,1.1);

    /* Sama glava. */
    glutSolidSphere(0.27,16,16);

    /* Oci. */
    setEyeMaterial();
    glPushMatrix();
       glTranslatef(0,0.27*cos(PI/4) - EPSILON,0);
       glPushMatrix();
          glTranslatef(0.27*0.5 - EPSILON,0,0.27*0.5 - EPSILON);
          glutSolidSphere(0.07,16,16);
       glPopMatrix();
       glPushMatrix();
          glTranslatef(-(0.27*cos(PI/4)*sin(PI/10) - EPSILON),0,0.27*cos(PI/4)*cos(PI/10) - EPSILON);
          glutSolidSphere(0.07,16,16);
       glPopMatrix();
    glPopMatrix();
    
    /* Tri vunene kuglice na iznad ociju. */
    setWoolMaterial();
    glPushMatrix();  /* Prva kuglica, na sredini. */
       glTranslatef(0,0.27,0);
       glutSolidSphere(0.1,16,16);
    glPopMatrix();
    glPushMatrix();  /* Druga kuglica, desno. */
       glTranslatef(cos(PI/5)*cos(PI/10)*0.27,cos(PI/10)*0.27,0);
       glutSolidSphere(0.1,16,16);
    glPopMatrix();
    glPushMatrix();  /* Treca kuglica, levo. */
       glTranslatef(-cos(PI/5)*cos(PI/10)*0.27,cos(PI/10)*0.27,0.1);
       glutSolidSphere(0.1,16,16);
    glPopMatrix();
}

void drawBody()  /* Crtanje tela. Znam da izgleda rogobatno, ali svaka 'ciljana' implementacija(tj. po nekoj petlji i sa nekim sistematicnim */
{                /* pozivanjem rand() funkcije je davala mnogo manje lep izgled ovce. 
                    Ovo je pravljeno naizmenicno izmenom parametara, pa ponovnim prevodjenjem programa. */
    addSphereCenterBody(-0.2,0);
    addSphereCenterBody(-0.4,0.15);
    addSphereCenterBody(-0.15,0.2);
    addSphereCenterBody(0.1,0.24);
    addSphereCenterBody(-0.5,0);
    addSphereCenterBody(0.08,0.07);
    addSphereCenterBody(0.3,0.2);
    addSphereCenterBody(0.25,0);
    addSphereCenterBody(0.42,0.085);
    addSphereCenterBody(-0.4,-0.2);
    addSphereCenterBody(-0.2,-0.25);
    addSphereCenterBody(-0.01,-0.23);
    addSphereCenterBody(0.3,-0.15);
    
    addSphereBodySymetric(0.05,0.12,0.056);
    addSphereBodySymetric(-0.15,0.02,0.056);
    addSphereBodySymetric(-0.23,0.33,0.06);
    addSphereBodySymetric(0.29,-0.15,0.066);
    addSphereBodySymetric(0.32,0.1,0.05);
    addSphereBodySymetric(0.01,0.33,0.023);
    addSphereBodySymetric(-0.2,0.2,0.1);
    addSphereBodySymetric(-0.3,0,0.1);
    addSphereBodySymetric(0.15,0.08,0.1);
    addSphereBodySymetric(0.012,0.012,0.087);
    addSphereBodySymetric(-0.033,0.01,0.08);
    addSphereBodySymetric(-0.01,0.02,0.17);
    addSphereBodySymetric(0.04,0.33,0.1);
    addSphereBodySymetric(0.25,0.25,0.1);
    addSphereBodySymetric(0.3,0.1,0.12);
}

void setSkinMaterial() /* Materijal za glavu i noge. */
{
    GLfloat ambient_coeffs[] = { 0.8, 0.8, 0.8, 1 };
    GLfloat diffuse_coeffs[] = { .368, .368, .244, 1 };
    GLfloat specular_coeffs[] = { 1, 1, 1, 1 };
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
}

void setEyeMaterial()  /* Materijal za oci. */
{
    GLfloat eambient_coeffs[] = { 0, 0, 0, 1 };
    GLfloat ediffuse_coeffs[] = { 0, 0, 0, 1 };
    GLfloat especular_coeffs[] = { 1, 1, 1, 1 };
    GLfloat eshininess = 100;
    glMaterialfv(GL_FRONT, GL_AMBIENT, eambient_coeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, ediffuse_coeffs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, especular_coeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, eshininess);
}

void setWoolMaterial()  /* Materijal za vuneno telo. */
{
    GLfloat oambient_coeffs[] = { 0.8, 0.8, 0.8, 1 };
    GLfloat odiffuse_coeffs[] = { .9, .9, .9, 1 };
    GLfloat ospecular_coeffs[] = { 1, 1, 1, 1 };
    GLfloat shininess = 15;
    glMaterialfv(GL_FRONT, GL_AMBIENT, oambient_coeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, odiffuse_coeffs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, ospecular_coeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}

void draw_object()  /* Funkcija koja kompletno iscrtava ovcu. */
{
    glScalef(0.5,0.5,0.5); /* Sve delove ovce posmatramo kao udeo lopte u koju je uklapamo. */
    /* Ovo je bio deo koda koji iscrtava crvenu zicanu sferu - znacilo dok smo pravili model ovce,
       da bi smo je uklopili u sferu zadatog poluprecnika (i time funkcije za detekciju kolizije bile bez problema primenjive i na nju) */
    /*------*/
     /* Prvo privremeno gasimo svetlo samo dok iscrtamo pomocnu zicanu sferu. */
    /*glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);
    glColor3f(1,0,0);
    glutWireSphere(1,16,16);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);*/
    /*-----*/

    setSkinMaterial(); /* Crtamo prvo kozne delove - noge i glavu (u okviru funkcije za icrtavanje glave ce se promeniti materijal, */
    drawLeg(0.1,-0.07,1,1);                                                          /* jer cemo i na glavi iscrtati vunene delove. */
    drawLeg(0.1,0.1,1,-1);
    drawLeg(-0.3,-0.07,-1,1);
    drawLeg(-0.3,0.1,-1,-1);
    glPushMatrix();
       drawHead();
    glPopMatrix();

    drawBody(); /* Sad crtamo vuneno telo. */   
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
    gluLookAt(sin(angle)*2, 0, cos(angle)*2, 0, 0, 0, 0, 1, 0);

    /* Ukljucuje se osvjetljenje i registruju parametri svetla. */
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    /* Kreira se objekat. */
    glPushMatrix();
       draw_object();
    glPopMatrix();

    /* Nova slika se salje na ekran. */
    glutSwapBuffers();
}
