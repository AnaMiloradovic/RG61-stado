/* Zaglavlje sa deklaracijama callback funkcija koje koristimo u programu. */
/* onDisplayFunction, onReshapeFunction, onKeyboardFunction 
   onKeyboardSpecialFunc(int key, int x, int y) - pomocu nje mozemo registrovati klik na strelice i druge specijalne tastere. */
#ifndef _CALLBACKS_
#define _CALLBACKS_

void onDisplayFunction();
void onReshapeFunction(int width, int height);
void onKeyboardFunction(unsigned char key, int x, int y);
void onKeyboardSpecialFunc(int key, int x, int y);
#endif