#include "SonarVDA.h"
#include "robot_kent.h"

SonarVDA sonar; //Céation d'un objet "sonar" de classe "SonarVDA"
volatile int compteur=0;

void obj_present(void) //Programme de gestion d'un obstacle
{
    Serial.println("Objet present");
    compteur++;
}

void obj_absent(void) //Programme de gestion de la disparition de l'obstacle
{
    Serial.println("Objet absent");
    compteur++;
}
//Constantes MOTEURS
#define ARRET 0
#define ETEINTE 0
#define ALLUMEE 1
#define stopit  analogWrite(PIN_MDR, 0);analogWrite(PIN_MDA, 0);analogWrite(PIN_MGR, 0);analogWrite(PIN_MGA, 0);
#define turn analogWrite(PIN_MDR, pot);analogWrite(PIN_MGA, pot);
#define turn2 analogWrite(PIN_MGR, pot);analogWrite(PIN_MDA, pot);
#define avance_stop digitalWrite(PIN_LED, HIGH);analogWrite(PIN_MGA, 15);analogWrite(PIN_MDA, 15);delay(1000);digitalWrite(PIN_LED, LOW);stopit


//Constantes BOUTON
#define APPUYE 1
#define RELACHE 0

//Regles de compilation
#define RUN 1

//interracxtion
#define ledon digitalWrite(PIN_LED, HIGH);
#define ledoff digitalWrite(PIN_LED, LOW);



// Variables globales :
unsigned char pot = 50; //200
unsigned char pot2 =50;

#define avance_gauche analogWrite(PIN_MGA, pot);analogWrite(PIN_MDA, pot/8);
#define avance_droite analogWrite(PIN_MDA, pot);analogWrite(PIN_MGA, pot/8);
#define avance analogWrite(PIN_MDA, pot);analogWrite(PIN_MGA, pot);
// Prototypes :
void gerer_appuie_start();


void etat_capteur() //pot&A 0->1023  //D 0->1
{
    Serial.print("/ A0 ");
    Serial.print(analogRead(PIN_AN0));
    Serial.print("/ A1 ");
    Serial.print(analogRead(PIN_AN1));
    Serial.print("/ A2 ");
    Serial.print(analogRead(PIN_AN2));
    Serial.print("/ A3 ");
    Serial.print(analogRead(PIN_AN3));
    Serial.print("/ D0 ");
    Serial.print(digitalRead(PIN_D0));
    Serial.print("/ D1 ");
    Serial.println(digitalRead(PIN_D1));
    Serial.print("Valeur Pot ");
    Serial.println(analogRead(PIN_POT));
    delay(1000);
}


/************************************************************************************/
/************************ INITIALISATION ********************************************/
/************************************************************************************/

int mode = 0;

void setup()  // Set pins as inputs
{
    pinMode(PIN_BTN, INPUT);
    pinMode(PIN_POT, INPUT);

    // Set pins as output
    pinMode(PIN_LED, OUTPUT);
    pinMode(PIN_MDR, OUTPUT);
    pinMode(PIN_MDA, OUTPUT);

    //Initialisation des sorties :
    digitalWrite(PIN_LED, ETEINTE);
    analogWrite(PIN_MDR, ARRET);
    analogWrite(PIN_MDA, ARRET);
    analogWrite(PIN_MGR, ARRET);
    analogWrite(PIN_MGA, ARRET);


    // initialize serial communications at 9600 bps
    Serial.begin(9600);

    // message pour la console
    Serial.println("Wait start"); // print fonctionne aussi mais println permet de sauter une ligne ÃƒÂ  la fin
    gerer_appuie_start();

    delay(500);
    // wait for 500ms

    mode = (analogRead(PIN_POT)>500)? 1 : 0;

    sonar.set_trigger(10, 20, obj_present, obj_absent);
    //Si un objet est présent à moins de 30cm la fonction "obj_present()" est appelée
    //Si aucun objet n'est détecté au dela de 50cm la fonction "obj_absent()" est appelée
    sonar.start(); // On démarre les mesures d'obstacle

}

int isblack(int c)
{
    if (c>500)
    {
        return 1;
    }
    return 0;
}


/************************************************************************************/
/********************** PROGRAMME EN BOUCLE *****************************************/
/************************************************************************************/
void loop()
{

    //while(1){  etat_capteur();  }
    //analogRead(PIN_POT)/4;
    char continuer=1;
    int delta,deltav;
    int d,g,mid;
    int c;
    int d_d,d_g;
    unsigned int etatd_d=0;
    unsigned int etatd_g=0;

#define tini 20
    int gg,dd;

    int dl;

    int etat_racourcis=0;

    for(; continuer;)
    {

        g = analogRead(PIN_AN3);
        d = analogRead(PIN_AN0);
        mid = analogRead(PIN_AN1);

        c = analogRead(PIN_AN2);
        d_d = digitalRead(PIN_D0)* isblack(g)*isblack(c);
        if(d_d)etatd_d=tini;
        d_g = digitalRead(PIN_D1)* isblack(d)*isblack(c);
        if(d_g)etatd_g=tini;

        if(etatd_g>0)etatd_g--;
        if(etatd_d>0)etatd_d--;

        if(etatd_g)ledon else ledoff


                //Delta
                delta = abs(d-g);
        deltav = pot2-pot2*(float)delta/1023;
        if(deltav>pot2)deltav=pot2;
        if(deltav<0)deltav=0;
        //Serial.println(deltav);




        if((mid>900 && g>900)||(mid>900 && d>900)) //Avancer !
        {
            //digitalWrite(PIN_LED, HIGH);
            //Serial.println("avance ");
            //analogWrite(PIN_MDA, pot);analogWrite(PIN_MGA, pot);
            if(g>d) //avance gauche
            {
                //Serial.println("gauche ");
#if RUN
                analogWrite(PIN_MDA, pot2);
                analogWrite(PIN_MGA, deltav);
#endif

            }
            else if(d>g)
            {
                //Serial.println("droite ");
#if RUN
                analogWrite(PIN_MGA, pot2);
                analogWrite(PIN_MDA, deltav);
#endif
            }
        }
        else //Tourner
        {

            if(g>d) //avance gauche
            {
                //Serial.println("gauche ");
#if RUN
                analogWrite(PIN_MDA, pot);
                analogWrite(PIN_MGA, 0);
#endif

            }
            else if(d>g)
            {
                //Serial.println("droite ");
#if RUN
                analogWrite(PIN_MGA, pot);
                analogWrite(PIN_MDA, 0);
#endif

            }
        }


        //CONDITIONS
        if(etatd_d && etatd_g)//d et g
        {
            //digitalWrite(PIN_LED, HIGH);
            for(int i=0; i<1000; i++)
            {
                g = analogRead(PIN_AN3);
                d = analogRead(PIN_AN0);
                delta = abs(d-g);
                deltav = pot2-pot2*(float)delta/1023;
                if(deltav>pot2)deltav=pot2;
                if(deltav<0)deltav=0;
                if(g>d) //avance gauche
                {
                    //Serial.println("gauche ");
#if RUN
                    analogWrite(PIN_MDA, pot2);
                    analogWrite(PIN_MGA, deltav);
#endif

                }
                else if(d>g)
                {
                    //Serial.println("droite ");
#if RUN
                    analogWrite(PIN_MGA, pot2);
                    analogWrite(PIN_MDA, deltav);
#endif
                }
            }
            //digitalWrite(PIN_LED, LOW);
            etatd_g =0;
            etatd_d =0;
            mid = analogRead(PIN_AN1);
            if(mid<500)
            {
                continuer=0;
            }
        }
        else if(abs(etatd_d-(tini/2))<2 && g>500 && c>500)  //360
        {
            stopit
            turn
            ledon
            delay(750);
            ledoff
            do
            {
                d = analogRead(PIN_AN0);
            }
            while(d<200);
            ledon
            delay(750);
            do
            {
                d = analogRead(PIN_AN0);
            }
            while(d<200);
            ledoff
            stopit

            turn2
            delay(50);
            stopit

            etatd_g =0;
            etatd_d =0;


        }
        else if(abs(etatd_g-(tini/2))<2 &&  d>500 && c>500)  //racourcis
        {

            if (mode)
            {
                analogWrite(PIN_MGA, 40);
                analogWrite(PIN_MDA, 40);
                delay(250);
                stopit
                analogWrite(PIN_MDA, pot);
                delay(1700);
                do
                {
                    mid = analogRead(PIN_AN2);
                }
                while(mid<200);
            }

            /*
                   while(!d_g)
                       {


                                    d_g = digitalRead(PIN_D1);
                                  g = analogRead(PIN_AN3);
                                  d = analogRead(PIN_AN0);
                                  delta = abs(d-g);
                                  deltav = pot2-pot2*(float)delta/1023;
                                  if(deltav>pot2)deltav=pot2;
                                  if(deltav<0)deltav=0;
                                  if(g>d) //avance gauche
                                  {
                                      //Serial.println("gauche ");
                  #if RUN
                                      analogWrite(PIN_MDA, pot2);
                                      analogWrite(PIN_MGA, deltav);
                  #endif

                                  }
                                  else if(d>g)
                                  {
                                      //Serial.println("droite ");
                  #if RUN
                                      analogWrite(PIN_MGA, pot2);
                                      analogWrite(PIN_MDA, deltav);
                  #endif
                                  }
                              }
                      stopit
                     delay(1000);

                      analogWrite(PIN_MGA, 40);analogWrite(PIN_MDA, 40);delay(900);
                      stopit delay(1000);
                      turn2 delay(400); do{g = analogRead(PIN_AN2);}while(g<200);
                      turn
                      delay(50);
                      stopit
                      //analogWrite(PIN_MGA, 40);analogWrite(PIN_MDA, 40);delay(250);
                      stopit delay(7000);

                       while(!d_g)
                              {


                                    d_g = digitalRead(PIN_D1);
                                  g = analogRead(PIN_AN3);
                                  d = analogRead(PIN_AN0);
                                  delta = abs(d-g);
                                  deltav = pot2-pot2*(float)delta/1023;
                                  if(deltav>pot2)deltav=pot2;
                                  if(deltav<0)deltav=0;
                                  if(g>d) //avance gauche
                                  {
                                      //Serial.println("gauche ");
                  #if RUN
                                      analogWrite(PIN_MDA, pot2);
                                      analogWrite(PIN_MGA, deltav);
                  #endif

                                  }
                                  else if(d>g)
                                  {
                                      //Serial.println("droite ");
                  #if RUN
                                      analogWrite(PIN_MGA, pot2);
                                      analogWrite(PIN_MDA, deltav);
                  #endif
                                  }
                              }
                                   stopit delay(1000);

                               turn2 delay(500); do{mid = analogRead(PIN_AN2);}while(mid<200);
                               stopit  delay(5000);

                    //stop
             */
        }

        if (sonar.get_last_measure() < 10 && sonar.get_last_measure() > 0 )
        {
            stopit   delay(5000);
        }
        //Ultrason

    }
    //OUT
    stopit
    while(1)
    {
        ;
    }
}

void gerer_appuie_start()
{
    while(digitalRead(PIN_BTN)==RELACHE);  // attendre.... ?
    digitalWrite(PIN_LED, ALLUMEE);      // LED .... ?
    while(digitalRead(PIN_BTN)==APPUYE);   // .... ?
    digitalWrite(PIN_LED, ETEINTE);      // .... ?
}
