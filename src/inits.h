#ifndef INITS_H
#define INITS_H

#define WINDOW_HEIGHT (800)    // Konstanta za duzinu prozora
#define WINDOW_WIDTH  (1000)   // Konstanta za sirinu prozora
#define WINDOW_POSITION_X (400)  // Konstanta za poziciju prozora
#define WINDOW_POSITION_Y (100)  // Konstanta za poziciju prozora
                              //TODO: Vrednosti koje smestaju prozor u centar naseg ekrana
#define MAX_NUM_OF_SHEEPS (5) // Maksimalan moguc broj ovaca 

#define NOT_TO_CHECK (0) // TODO Ako se utvrdi kolizija i-te ovce(kugle), nije potrebno azurirati smer njene brzine.
#define TO_CHECK (1)    //TODO  Ako se utvrdi kolizija i-te ovce(kugle), azurira se njena brzina.

//Dimenzje livade i poluprecnik kugle (u svetskom koordinatnom sistemu)
#define MEADOWDIMENSION_X (5.0)
#define MEADOWDIMENSION_Y (0.25) 
#define MEADOWDIMENSION_Z (10.0) 
#define RADIUS (0.5)


/* HACK:
#define MAX_POSSIBLE_CLOSED_SURFACES (200) //Pretpostavljamo da ne moze biti prevelik broj pravougaonika u okviru zatvorene povrsi
*/


typedef struct {
    float pX,pZ, // Pozicija na terenu
           vX, vZ,   // X i Z komponente brzine (zavisice i od samog nivoa, od koga zavisi intenzitet ukupne brzine,
              alfa,   // ali i od ugla alfa koji odredjuje odnos ove dve brzine.
                     // Ako je v ukupna brzina, onda je vX = v*cos(alfa) i vZ = v*sin(alfa).
                     // Ipak, umesto da cuvamo informacije o ukupnoj brzini, mi pamtimo obe komponente brzine
                     // jer su funkcije sin i cos vremenski skupe da bi se svaki put racunale.
    angle,w_angle;  // Tekuci ugao rotacije i ugaona brzina kotrljanja kugle
} BALL;  //Struktura u kojoj cuvamo informacije o ovcama(ili kuglama). Polje angle ima razlicito znacenje kod ove dve implementacije:
// Kod kugle ona predstavlja ugao za koji je zarotirana kugle, dok kod ovaca ona predstavlja ugao sinusa koji odredjuje njenu
// y-koordinatu (tj. koliko je ovcica 'poskocila' u vazduhu)

typedef struct {
    float pX, pY, pZ, vX, vZ;
} CLOSER; // TODO Struktura u kojoj cuvamo informacije o objektu koji zatvara ovce.


typedef struct {
    float pX,pY,pZ;
} POINT; // Struktura za cuvanje informacija o tackama na terenu (neophodno za iscrtavanje putanje kojom zatvaramo povrsinu na terenu)


/* HACK:
typedef struct {
    float minX, minZ, maxX, maxZ;
} SURFACE; // Struktura u za cuvanje minimalnih i maksimalnih x i z koordinata u okviru pravougaonog dela
           // zatvorene povrsi
*/


void gameDataInitialization();  //Funkcija za unos podataka o igri
void graphicsInitialization(); //Neophodne graficke inicijalizacije
void initialPos(); //inicijalizacije pocetnu poziciju kugli

#endif