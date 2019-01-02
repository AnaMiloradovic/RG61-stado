#ifndef _SHEEP_H_
#define _SHEEP_H_

#define R_LEG 0.075

void addSphereCenterBody(float x, float y);
void addSphereBodySymetric(float x, float y, float z);
void drawSheep(float angle);
void drawLeg(float posX,float posZ, float backLeg, float oppositeSide);
void drawHead();
void drawBody();
void setSkinMaterial();
void setEyeMaterial();
void setWoolMaterial();

#endif