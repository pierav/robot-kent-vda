
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


#if STATS
etat_capteur();

#if DRAW_DL
//DL : Distance du centre de la ligne
dl= ((unsigned int)(((1000*3-g-d-mid)/3)>>3))<<3;
if(g>d) {
  dl=-dl;
  for(j=-1024;j<1024;j+=30){
    if(j>dl)Serial.print("=");else Serial.print(" ");
  }
}
else{
  for(j=-1024;j<1024;j+=30){
    if(j<dl)Serial.print("=");
  }
}
Serial.println(dl);
#endif


#if DRAW_APROX
//Approche de la ligne
 for(j=1020;j>=0;j-=20){
    if(j<g)Serial.print("=");
    else Serial.print(" ");
  }
 for(j=0;j<=1020;j+=20){
    if(j<d)Serial.print("=");
    else Serial.print(" ");
  }
  Serial.println("");
#endif


#if DRAW_CAPTOR
//Niveau des 3 capteurs
for(j=40;j<=1000;j+=20){
     if(j<g)Serial.print("=");
     else Serial.print(" ");
  }
  Serial.print("|");
  for(j=40;j<=1000;j+=20){
     if(j<mid)Serial.print("=");
     else Serial.print(" ");
  }
  Serial.print("|");

  for(j=40;j<=1000;j+=20){
     if(j<d-100)Serial.print("=");
     else Serial.print(" ");
  }
  Serial.println("");
#endif
