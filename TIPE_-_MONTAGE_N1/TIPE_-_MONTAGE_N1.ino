// TIPE 2020/2021 : MARCAL THOMAS
// MONTAGE N°1

// Initialiser l'écran LCD -> Inclusion des librairies utilisées
#include <Wire.h>
#include <LiquidCrystal_I2C.h>      // Ajout de la librairie pour afficheur écran LCD 
LiquidCrystal_I2C lcd(0x27, 20, 4); 

// Definition des variables 
int Capteur = A3;         // Définition de la pin A3 de la carte en tant que variable => pin à laquelle la sortie de l'anémomètre est connectée 
int ValeurCapteur0 = 120; // Valeur de sécurité trouver expérimentalement => seuil pour une valeur de vitesse du vent faible
int Delai = 1000;         // Delai d'affichage de 1 seconde
float VitesseVent = 0;
float VitesseVentF = 0;
float Temps = 0.0;
float TempsSec = 0.0;

// Caractéristique ANEMOMETRE ADAFRUIT 
float TensionMin = 0.4;  // Tension de sortie minimale de l'anémomètre en V. 
float VitVentMin = 0.01; // Vitesse du vent en m/s correspondant à la tension minimale
float TensionMax = 2;    // Tension de sortie maximale de l'anémomètre en V. 
float VitVentMax = 32.4; // Vitesse du vent en m/s correspondant à la tension maximale

void setup() { // Debut de la fonction setup
  
  lcd.init();      // Initialisation de l'écran
  lcd.backlight();

// FONCTIONALITE / PRESENTATION MONTAGE
  lcd.setCursor (0, 0); 
  lcd.print ("- ANEMOMETRE   -"); 
  lcd.setCursor (0, 1); 
  lcd.print ("- MONTAGE N.1  -"); 
  delay(2000); // Pause de 2 secondes
  
  lcd.clear (); 
  delay(100);  

  Serial.begin (9600);// Définit le débit de données en bits par seconde (bauds) pour la transmission de données série
} // Fin de la fonction setup()

void loop() { // Debut de la fonction loop()

  int ValeurCapteur = analogRead(Capteur); //Lit la valeur de la pin analogique A3 => cartes Arduino contient un convertisseur analogique-numérique 10 bits multicanaux donc renvoie les tensions d'entrée entre 0 et la tension de fonctionnement 5V en valeurs entières comprises entre 0 et 1023. 
  float TensionSortie = ValeurCapteur * (5.0/1023.0); // Convertit la valeur du capteur en tension réelle
  Temps = millis();      // Renvois le temps en millisecondes 
  TempsSec = Temps/1000; // Renvois le temps en secondes 
  if ((TensionSortie < TensionMin) or (ValeurCapteur0 > ValeurCapteur)) { // Conditions non respectées ne permettant pas d'afficher une valeur précise du vent
    VitesseVentF = VitVentMin; // Vitesse du vent renvoyé est nulle
  } else { 
    VitesseVent = ((TensionSortie - TensionMin) * VitVentMax / (TensionMax - TensionMin)); // Pour les tensions supérieures à la valeur minimale, utilisez la relation donnée par le constructeur pour calculer la vitesse du vent. 
    VitesseVentF = 0.8776*VitesseVent*3.6-1,3458; // Application de la fonction étallonage et conversion en km/h
  }

// AFFICHAGE POUR L'ECRAN LCD
  lcd.setCursor (0, 0); 
  lcd.print ("Vitesse du vent"); 
  lcd.setCursor (0, 1); 
  lcd.print (VitesseVentF);
  lcd.setCursor (6, 1); 
  lcd.print ("km/h");
  
// AFFICHAGE POUR EXTRAIRE LES DONNEES DE VITESSE ET DE TEMPS
  Serial.print(VitesseVentF);
  Serial.print(",");
  Serial.println(TempsSec);
  
  delay(Delai); // Delai de 1 secondes
} // Fin de la fonction loop()

// FIN
