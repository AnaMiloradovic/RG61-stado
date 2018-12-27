#ifndef INITS_H
#define INITS_H

#define WINDOW_HEIGHT (800)    // Konstanta za duzinu prozora
#define WINDOW_WIDTH  (1000)   // Konstanta za sirinu prozora
#define WINDOW_POSITION_X (400)  // Konstanta za poziciju prozora
#define WINDOW_POSITION_Y (100)  // Konstanta za poziciju prozora
                              //TODO: Vrednosti koje smestaju prozor u centar naseg ekrana
#define MAX_NUM_OF_SHEEPS (5) // Maksimalan moguc broj ovaca 

#define NOT_TO_CHECK (0) // Ne proverava se kolizija sa i-tom kuglom u nizu
#define TO_CHECK (1)    // Proverava se kolizija sa i-tom kuglom u nizu

//Dimenzje livade i poluprecnik kugle (u svetskom koordinatnom sistemu)
#define MEADOWDIMENSION_X (5)
#define MEADOWDIMENSION_Y (0.25) 
#define MEADOWDIMENSION_Z (10) 
#define RADIUS (0.5)



typedef struct {
    float pX,pZ, // Pozicija na terenu
         minX, maxX, // Granice u okviru kojih je (kugla),
         minZ, maxZ, // inicijalno su velicine terena , trebace nam kada budemo zagradjivali
           vX, vZ,   // X i Z komponente brzine (zavisice i od samog nivoa, od koga zavisi intenzitet ukupne brzine,
              alfa,   // ali i od ugla alfa koji odredjuje odnos ove dve brzine.
                     // Ako je v ukupna brzina, onda je vX = v*cos(alfa) i vZ = v*sin(alfa).
                     // Ipak, umesto da cuvamo informacije o ukupnoj brzini, mi pamtimo obe komponente brzine
                     // jer su funkcije sin i cos vremenski skupe da bi se svaki put racunale.
    angle,w_angle;  // Tekuci ugao rotacije i ugaona brzina kotrljanja kugle
} BALL;
                               
void gameDataInitialization();  //Funkcija za unos podataka o igri
void graphicsInitialization(); //Neophodne graficke inicijalizacije
void initialPos(); //inicijalizacije pocetnu poziciju kugli

#endif