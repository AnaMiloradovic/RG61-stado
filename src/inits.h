/* Zaglavlje sa deklaracijama inicijalizacionih funkcija, deklaracijama struktura osnovnih objekata u igri
   i sa simbolickim konstantama. */
#ifndef INITS_H
#define INITS_H

#define WINDOW_HEIGHT (800)    /* Konstanta za duzinu prozora */
#define WINDOW_WIDTH  (1000)   /* Konstanta za sirinu prozora */
#define WINDOW_POSITION_X (400)  /* Konstanta za poziciju prozora na ekranu. */
#define WINDOW_POSITION_Y (100)  /* Konstanta za poziciju prozora na ekranu. */
                            
#define MAX_NUM_OF_SHEEPS (5) /* Maksimalan moguc broj ovaca */

#define PI (3.141592653589793) /* Konstanta PI */

/* Dimenzje livade (u svetskom koordinatnom sistemu) */
#define MEADOWDIMENSION_X (5.0)
#define MEADOWDIMENSION_Y (0.25) 
#define MEADOWDIMENSION_Z (10.0) 

#define RADIUS (0.5) /* Poluprecnik sfere opisane oko ovce, u funkciji za detekciju kolizije ovce i dalje apstrahujemo kuglama. */ 
#define TIME_OUT (75*50) /* Maksimalan moguc broj pozivanja tajmera - kada se ovoliko puta pozove funkcija za kretanje ovaca - vreme za igru je isteklo. */
#define NIGHT_NEAR (60*50) /* Posle ovoliko pozivanja tajmera blizu je kraj igre. */
#define TIMER_INTERVAL (20) /* Na ovoliko milisekundi ce se pozivati tajmer. */
#define TIMER_ID_ROLLING (0)  /* ID tajmera vezanog za funkciju kretanja ovaca. */

typedef struct { /* Struktura ovcice */
    float pX,pZ, /* Pozicija na terenu */
           vX, vZ,   /* X i Z komponente brzine (zavise i od samog nivoa, od koga zavisi intenzitet ukupne brzine, */
              alfa,  /* a od konstante alfa zavisi odnos izmedju te 2 komponente. */
    angle; /* Angle - koliko ce jos ovcica biti podignuta iznad terena. 
            Zapravo je ta velicina izrazena srazmerno sinusu ovog ugla, detaljnije o ovome je u fajlu sheep.c . */
} BALL;

typedef struct {
    float pX, pY, pZ, vX, vZ;
} CLOSER; /* Struktura u kojoj cuvemo informacije o zatvarajucem objektu. */

typedef struct {
    float pX,pY,pZ;
} POINT; /* Struktura za cuvanje informacija o tackama na terenu. */

void gameDataInitialization();  /* Funkcija za inicijalizaciju podataka o igri. */
void graphicsInitialization();  /* Funkcija za graficku inicijalizaciju igre. */
void initialPos(); /* Inicijalizacija pocetnih polozaja i brzina zatvarajuceg objekta i ovci. */

#define PERCENT_PER_SHEEP (10) /* Default-an maksimalan dozvoljeni slobodan procenat povrsine po ovci. (za pobedu) */
#endif                         /* Za vise nivoe ce u proverama pobede biti smanjivan. */