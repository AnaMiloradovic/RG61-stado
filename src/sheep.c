#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

#include "sheep.h"

#ifndef PI
#define PI 3.1415926535
#endif

#ifndef EPSILON
#define EPSILON 0.01
#endif

void addSphereCenterBody(float x, float y)
{
    glPushMatrix();
    glTranslatef(x,y,0);
    glutSolidSphere(0.3,16,16);
    glPopMatrix();
}

void addSphereBodySymetric(float x, float y, float z)
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

void drawLeg(float posX,float posZ, float backLeg, float oppositeSide)
{
     setSkinMaterial();
     glPushMatrix();
     glScalef(1,1,oppositeSide);
     glTranslatef(posX,0,posZ);
     glRotatef(backLeg*20,-1,0,1);
     float u,h;
     glBegin(GL_TRIANGLE_STRIP);
     for(h=0;h>= -0.5;h-=PI/20)
        for(u=0;u<2*PI+EPSILON;u+=PI/20)
        {
            glNormal3f(cos(u),0,sin(u));
            glVertex3f(R_LEG*cos(u),h,R_LEG*sin(u));
            glVertex3f(R_LEG*cos(u),h-PI/20,R_LEG*sin(u));
        }
    glEnd();
    glTranslatef(0,-0.63,0);
    glutSolidSphere(R_LEG,16,16);
    glPopMatrix();
}

void drawHead()
{
    setSkinMaterial();
    glTranslatef(0.4,0.2,0.27);
    glScalef(1.1,1,1.1);

    glutSolidSphere(0.27,16,16);
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
    // Vrh
    setWoolMaterial();
    
    glPushMatrix();
    glTranslatef(0,0.27,0);
    glutSolidSphere(0.1,16,16);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(cos(PI/5)*cos(PI/10)*0.27,cos(PI/10)*0.27,0);
    glutSolidSphere(0.1,16,16);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-cos(PI/5)*cos(PI/10)*0.27,cos(PI/10)*0.27,0.1);
    glutSolidSphere(0.1,16,16);
    glPopMatrix();
}

void drawBody()
{
    // Telo
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

void setSkinMaterial()
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

void setEyeMaterial()
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

void setWoolMaterial()
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

void drawSheep(float angle)
{
    glScalef(0.5,0.5,0.5);
    /*glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);
    glColor3f(1,0,0);
    glutWireSphere(1,16,16);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);*/    
    

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

