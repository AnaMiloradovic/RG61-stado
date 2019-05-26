 /* Zaglavlje sa deklaracijama svih funkcija koje koristimo pri crtanju ovce. */
#ifndef _SHEEP_H_
#define _SHEEP_H_

#define R_LEG 0.075 /* Poluprecnik baze valjka koji cini nogu i poluprecnika polusfere na vrhu. */

#ifndef PI
#define PI 3.1415926535 /* Ako vec na drugom mestu nije definisan broj PI, dodajemo ga. */
#endif

#ifndef EPSILON
#define EPSILON 0.01    /* Slicno i za malu vrednost EPSILON. */
#endif

void drawSheep(float angle);  /* Omotac funkcija za crtanje citave ovcice. */
void addSphereCenterBody(float x, float y); /* Dodavanje loptice vune na sredinu tela. */
void addSphereBodySymetric(float x, float y, float z); /* Bocno dodavanje loptice vune, po jedna s obe strane. */
void drawLeg(float posX,float posZ, float backLeg, float oppositeSide); /* Crtanje noge. */
void drawHead(); /* Crtanje glave. */
void drawBody(); /* Crtanje tela. */

void setSkinMaterial(); /* Postavljanje materijala za kozu ovce. */
void setEyeMaterial();  /* Postavljanje materijala na oci. */
void setWoolMaterial(); /* Postavljanje vunenog materijala. */

#endif