// TIPE 2020/2021 : MARCAL THOMAS 
// MONTAGE N°2

// Initialiser l'écran LCD -> Inclusion des librairies utilisées
#include "LiquidCrystal.h"             // Ajout de la librairie pour afficheur écran LCD 
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // Initialise les pins de l'écran LCD pour son fonctionnement

// Definition des variables 
float R = 0.115;            // Correspond au rayon des bras de l'anémometre = 11,5 cm
float Temps0 = 0.0;         // Temps du passage de l'aimant
float VitTot = 0.0;         // Variable stockant la somme des vitesses pour les n tours
float VitMoy = 0.0;         // Variable de stockage renvoyant la moyenne des vitesses sur les n tours
int NbDeTour = 0;           // Compteur du nombre de tour
int Position0;              // Instant du passage de l'aimant
int Delai = 1000;           // Delai d'affichage (fixé à 1 seconde)
  
void setup() { // Debut de la fonction setup
  
  lcd.begin (16, 2); // Initialise le LCD avec 16 colonnes x 2 lignes
  delay(100);        // Pause pour laisser temps d'initialisation
  
// FONCTIONALITE / PRESENTATION MONTAGE
  lcd.setCursor (0, 0); 
  lcd.print ("- ANEMOMETRE   -"); 
  lcd.setCursor (0, 1); 
  lcd.print ("- MONTAGE N.2  -"); 
  delay(2000); // Pause de 2 secondes
  
  lcd.clear (); 
  delay(100);

// INITIALISE LE SYSTEME
  pinMode(8, INPUT);          // Interrupteur Reed branché à la pin 8
  pinMode(13,OUTPUT);         // Configuration pin 13 qui permet de régler si MODE OUTPUT : 5V si HIGH et 0V si LOW => REGLER EN MODE HIGH
  Serial.begin(9600);         // Définit le débit de données en bits par seconde (bauds) pour la transmission de données série
  Position0 = digitalRead(8); // Vérification du passage de l'aimant : HIGH si il passe (présence de l'aimant qui active l'interupteur reed) et LOW sinon 
} // Fin de la fonction setup()

void loop() { // Debut de la fonction loop()
  
  unsigned long Etat;
  unsigned long Temps1;
  float Duree;
  float VitVent = 0.0;    // Variable de stockage
  float VitVentEta = 0.0; // Variable de stockage
  float VitF = 0.0;       // Variable donnant la moyenne du vent pour 10 tours 
  
  Etat = digitalRead(8);  // Vérification du passage de l'aimant : HIGH si il passe (présence de l'aimant qui active l'interupteur reed) et LOW sinon 
  digitalWrite(13,Etat);  // Carcatérise la pin 13 et son état de fonctionnement

  if ((Etat) && (!Position0)) {       // Comparaison qui marche si les deux propositions sont vraies, cad que l'aimant a réalisé un tour  
    if (Temps0 >= 0) {                // Comparaison de sécurité + Permet de bien definir le debut de notre tour    
      Temps1 = millis();              // Mesure du temps à l'instant de l'aimant (en milliseconde)
      Duree = (Temps1 - Temps0)/1000; // Durée du tour qui vient de se réaliser (en seconde)
      VitVent = 3.6*2*3.14*R/Duree;   
      VitVentEta = 0.1388*VitVent+0,9253; // Application de la fonction d'étalonnage trouvée expérimentalement
// Duree en senconde equivaut à : Duree(en milliseconde)/1000
// Vitesse en m/s donne périmètre/temps = 2*pi*R/Temps
// Conversion en kilometre/heure en multipliant par 3,6
      }
      VitTot += VitVentEta; // Stocke en sommant les vitesses du vents obtenues
      if (Etat && HIGH) {   // Comparaison qui permet de compter le nb de tour, donc passage de l'aimant
        NbDeTour += 1;      // Compteur de tour
        if (0 == NbDeTour % 20) { // Chaque passage de l'aimant ajoute +2 tour => permet de compter pour 10 tour
          VitF = VitTot/10; // Vitesse moyenne du vent pour 10 tours
          VitMoy=VitF;      // Variable permettant de renvoyer la vitesse moyenne pour l'affichage de l'écran LCD
          VitTot=0.0;       // Reinitiale de la variable stockage de la vitesse du vent tous les 10 tours
        }
      }
      // AFFICHAGE POUR L'ECRAN LCD
      lcd.setCursor (0, 0); 
      lcd.print ("Vitesse du vent");  
      lcd.setCursor (0, 1); 
      lcd.print (VitMoy); 
      lcd.setCursor (7, 1); 
      lcd.print ("km/h");  
    
// Récurrence de notre procédé
    Temps0 = Temps1;  // Nouvelle affectation de la variable pour itérer le resultat
  }
  Position0 = Etat;  // Nouvelle affectation de la variable pour itérer le resultat
} // Fin de la fonction loop()

// FIN
