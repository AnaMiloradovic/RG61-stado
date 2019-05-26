/* Izvorna datoteka sa implementacijama funkcija za iscrtavanje ovcica i postavljanje materijala
   na razlicite delove tela. Iako se skoro sve funkcije za postavljanje materijala nalaze u datoteci
   light_and_materials, ovde su namerno zasebno odvojene u zaseban modul da bi crtanje ovcica moglo
   da se ukljuci i u druge programe. */
#include <GL/glut.h>
#include <math.h>

#include "sheep.h"

/* NAPOMENA: Znam da se u ovom kodu i jos par izvornih datoteka nalazi mnogo konstanti koje nisu pozeljne u praksi, 
   pri programiranju su naizmenicno podesavani parametri i prevodjen program, sve do postizanja zeljenog izgleda. */

void addSphereCenterBody(float x, float y) /* Dodavanje loptice vune na sredinu tela. */
{                                          /* Parametri x i y su odgovarajuce koordinate centra loptica. (kod ovih je z uvek jednaka 0) */
    glPushMatrix();
       glTranslatef(x,y,0);
       glutSolidSphere(0.3,16,16);
    glPopMatrix();
}

void addSphereBodySymetric(float x, float y, float z) /* Bocno dodavanje loptice vune, po jedna s obe strane. Parametri odgovarajuce koordinate centara. */
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

/* Crtanje noge. (cilindra sa dodatom polusferom na donji kraj)
   Parametri: posX i posZ koordinate vrha noge (gornje baze),
   backleg - ugao za koliki je noga zarotirana unazad (oko z-ose),
   oppositeSide - ugao za koliki je noga zarotirana u stranu (oko x-ose) */
void drawLeg(float posX,float posZ, float backLeg, float oppositeSide) 
{  
     setSkinMaterial();
     glPushMatrix();
        glScalef(1,1,oppositeSide); /* Namestamo odgovarajuci polozaj koordinatnog sistema za parametrizaciju valjka. */
        glTranslatef(posX,0,posZ);
        glRotatef(backLeg*20,-1,0,1);

        float u,h;
        glBegin(GL_TRIANGLE_STRIP); /* Parametrizacija valjka. */
           for(h=0;h>= -0.5;h-=PI/20)
              for(u=0;u<2*PI+EPSILON;u+=PI/20)
              {
                 glNormal3f(cos(u),0,sin(u));
                 glVertex3f(R_LEG*cos(u),h,R_LEG*sin(u));
                 glVertex3f(R_LEG*cos(u),h-PI/20,R_LEG*sin(u));
              }
        glEnd();

        glTranslatef(0,-0.63,0);
        glutSolidSphere(R_LEG,16,16); /* Crtamo polusferu na donjem kraju noge za verniji izgled. Ne treba nam clipping - ravan, jer se gornji kraj sfere svakako ne vidi. */
    glPopMatrix();
}

void drawHead() /* Crtamo ovcinu glavu. */
{
    setSkinMaterial(); /* Crtamo glavni deo glave. */
    glTranslatef(0.4,0.2,0.27);
    glScalef(1.1,1,1.1);
    glutSolidSphere(0.27,16,16);

    setEyeMaterial(); /* Crtamo oci. */
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
    
    setWoolMaterial(); /* Crtamo tri vunene loptice iznad ociju. */
    glPushMatrix();
       glTranslatef(0,0.27,0);   /* Loptica na sredini. */
       glutSolidSphere(0.1,16,16);
    glPopMatrix();
    glPushMatrix();              /*  Desna loptica. */
       glTranslatef(cos(PI/5)*cos(PI/10)*0.27,cos(PI/10)*0.27,0);
       glutSolidSphere(0.1,16,16);
    glPopMatrix();
    glPushMatrix();              /* Leva loptica. */
       glTranslatef(-cos(PI/5)*cos(PI/10)*0.27,cos(PI/10)*0.27,0.1);
       glutSolidSphere(0.1,16,16);
    glPopMatrix();
}

void drawBody()  /* Crtamo ovcino telo, lopticu po lopticu. */
{
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

void setSkinMaterial() /* Materijal ovcine koze. */
{
    GLfloat ambient_coeffs[] = { 0.8, 0.8, 0.8, 1 };
    GLfloat diffuse_coeffs[] = { 0.568, 0.468, 0.244, 1 };
    GLfloat specular_coeffs[] = { 1, 1, 1, 1 };
    GLfloat shininess = 30;
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
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

void setWoolMaterial() /* Materijal za vunu. */
{
    GLfloat oambient_coeffs[] = { 0.8, 0.8, 0.8, 1 };
    GLfloat odiffuse_coeffs[] = { .9, .9, .9, 1 };
    GLfloat ospecular_coeffs[] = { 1, 1, 1, 1 };
    GLfloat shininess = 15;
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, oambient_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, odiffuse_coeffs);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ospecular_coeffs);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}

void drawSheep(float angle)  /* Omotac funkcija za crtanje citave ovce. */
{
    glScalef(0.5,0.5,0.5);
    glTranslatef(0,0.5,0);
    glTranslatef(0,0.25*sin(angle),0);
    drawLeg(0.1,-0.07,1,1);
    drawLeg(0.1,0.1,1,-1);
    drawLeg(-0.3,-0.07,-1,1);
    drawLeg(-0.3,0.1,-1,-1);
    
    glPushMatrix();
       drawHead();
    glPopMatrix();

    drawBody();  
}

