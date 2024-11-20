#define CHAN_KEY 0
#define CHAN_NewKEY 1
#define CHAN_NewsKEY 2
#define CHAN_NewssKEY 3

#include <M5Atom.h>
#include <MicroOscSlip.h>
#include <M5_PbHub.h>
#include <VL53L0X.h>

CRGB pixel;
unsigned long monChronoMessages;
MicroOscSlip<128> monOsc(&Serial);
M5_PbHub myPbHub;
VL53L0X myTOF;

int maLectureKeyPrecedente;
int maLectureKeyPrecedentes;
int maLectureKeyPrecedentess;
int maLectureKeyPrecedentesss;
int etatPlay;

int sequenceAppui[4] = {0}; // HYPER IMPORTANT. LA SÉQUENCE JOUEUR QUI SERA CRÉÉ
int sequenceIndex = 0; // ON EST RENDU A QUEL ENDROIT DANS LA SEQUENCE




int sequence[] = {CHAN_NewsKEY, CHAN_NewsKEY, CHAN_NewssKEY, CHAN_NewKEY}; // VOICI LE CODE COULEUR SÉQUENCE ! SE QU'ON MODIFIE ICI ON DOIT AUSSI ÊTRE MODIFIÉ DANS sequenceResult[4];
int sequenceLength = sizeof(sequence) / sizeof(sequence[0]); 

// Variables pour contrôler la lecture de la séquence
int currentIndex = 0; //Quel index sommes nous dans la sequence pour la démo
bool hasExecuted = false; // La démo du début. Ceci est le déclancheur pour que l'énigme commence

void setup() {
  M5.begin(false, false, false);
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(&pixel, 1);  // Ajouter le pixel du M5Atom à FastLED
  Serial.begin(115200);

  unsigned long chronoDepart = millis();
  while (millis() - chronoDepart < 5000) {
    pixel = CRGB(255, 255, 255);
    FastLED.show();
    delay(100);

    pixel = CRGB(0, 0, 0);
    FastLED.show();
    delay(100);
  }

  pixel = CRGB(0, 0, 0);
  FastLED.show();

  Wire.begin();
  myPbHub.begin();
  myPbHub.setPixelCount(CHAN_KEY, 1);
  myPbHub.setPixelCount(CHAN_NewKEY, 1);
  myPbHub.setPixelCount(CHAN_NewsKEY, 1);
  myPbHub.setPixelCount(CHAN_NewssKEY, 1);

  myTOF.init();
  myTOF.setTimeout(500);
  myTOF.startContinuous();
}

void maReceptionMessageOsc(MicroOscMessage& oscMessage) {
  if (oscMessage.checkOscAddress("/master/vu")) {
    float vu = oscMessage.nextAsFloat();
    int niveau = floor(vu * 255.0);
    pixel = CRGB(niveau, niveau, niveau);
    FastLED.show();
  }
}

void loop() {
  M5.update();
  monOsc.onOscMessageReceived(maReceptionMessageOsc);

  if (millis() - monChronoMessages >= 20) {
    monChronoMessages = millis();

    int maLectureKey = myPbHub.digitalRead(CHAN_KEY);//4 boutons
    int maLectureKeys = myPbHub.digitalRead(CHAN_NewKEY);
    int maLectureKeyss = myPbHub.digitalRead(CHAN_NewsKEY);
    int maLectureKeysss = myPbHub.digitalRead(CHAN_NewssKEY);

    if (maLectureKeyPrecedente != maLectureKey) {
      if (maLectureKey == 0) {
        sequenceAppui[sequenceIndex] = sequenceAppui[sequenceIndex] = 1;//VALEUR DU VERT EST 1
        sequenceIndex++;
        myPbHub.setPixelColor(CHAN_KEY, 0, 0, 255, 0);
      }
      if (maLectureKey == 1) {
        myPbHub.setPixelColor(CHAN_KEY, 0, 0, 0, 0);
      }
    }

    if (maLectureKeyPrecedentes != maLectureKeys) {
      if (maLectureKeys == 0) {
        sequenceAppui[sequenceIndex] = sequenceAppui[sequenceIndex] = 2;// VALEUR DU BLEU EST 2
        sequenceIndex++;
        myPbHub.setPixelColor(CHAN_NewKEY, 0, 0, 0, 255);
      }
      if (maLectureKeys == 1) {
        myPbHub.setPixelColor(CHAN_NewKEY, 0, 0, 0, 0);
      }
    }

    if (maLectureKeyPrecedentess != maLectureKeyss) {
      if (maLectureKeyss == 0) {
        sequenceAppui[sequenceIndex] = sequenceAppui[sequenceIndex] = 3; //VALEUR DU ROUGE EST 3
        sequenceIndex++;
        myPbHub.setPixelColor(CHAN_NewsKEY, 0, 255, 0, 0);
      }
      if (maLectureKeyss == 1) {
        myPbHub.setPixelColor(CHAN_NewsKEY, 0, 0, 0, 0);
      }
    }

    if (maLectureKeyPrecedentesss != maLectureKeysss) {
      if (maLectureKeysss == 0) {
        sequenceAppui[sequenceIndex] = sequenceAppui[sequenceIndex] = 4; //VALEUR DU JAUNE EST 4
        sequenceIndex++;
        myPbHub.setPixelColor(CHAN_NewssKEY, 0, 255, 245, 0);
      }
      if (maLectureKeysss == 1) {
        myPbHub.setPixelColor(CHAN_NewssKEY, 0, 0, 0, 0);
      }
    }

    maLectureKeyPrecedente = maLectureKey;// En gros c'est les détecteurs si clicks pour 1 valeur et non 50 à la secondes
    maLectureKeyPrecedentes = maLectureKeys;
    maLectureKeyPrecedentess = maLectureKeyss;
    maLectureKeyPrecedentesss = maLectureKeysss;

    if (!hasExecuted) {// NE PAS JOUER LA BOUCLE EN RÉPÉTITION. COmme une petite assurance pour éviter que ce soit un vrai show de lumière
      if (currentIndex >= sequenceLength) {
        currentIndex = 0;
      }
      delay(250);
      myPbHub.setPixelColor(sequence[currentIndex], 0, 0, 0, 0); // COMMENCE SANS COULEUR
      delay(450);
      myPbHub.setPixelColor(sequence[currentIndex], 0, 255, 0, 0); // SEQ1 A VOIR LES COULEURS
      delay(300);
      myPbHub.setPixelColor(sequence[currentIndex], 0, 0, 0, 0);
      delay(450);
      currentIndex++;
      myPbHub.setPixelColor(sequence[currentIndex], 0, 255, 0, 0); // SEQ2
      delay(300);
      myPbHub.setPixelColor(sequence[currentIndex], 0, 0, 0, 0);
      delay(450);
      currentIndex++;
      myPbHub.setPixelColor(sequence[currentIndex], 0, 255, 245, 0); // SEQ3
      delay(300);
      myPbHub.setPixelColor(sequence[currentIndex], 0, 0, 0, 0);
      delay(450);
      currentIndex++;
      myPbHub.setPixelColor(sequence[currentIndex], 0, 0, 0, 255); // SEQ4
      delay(300);
      myPbHub.setPixelColor(sequence[currentIndex], 0, 0, 0, 0);

      hasExecuted = true; // NE PAS RÉPÉTER CETTE SÉQUENCE EN BOUCLE
    }
//VERT = 1
//BLEU = 2
//ROUGE = 3
//JAUNE = 4
   int sequenceResult[4] = {3, 3, 4, 2}; // Bonne séquence

if (sequenceIndex == 4) {
  // Voir si bon resultat
  if (sequenceAppui[0] == sequenceResult[0] &&
      sequenceAppui[1] == sequenceResult[1] &&
      sequenceAppui[2] == sequenceResult[2] &&
      sequenceAppui[3] == sequenceResult[3]) {
    myPbHub.setPixelColor(CHAN_KEY, 0, 0, 255, 0); // Séquence correcte, LED verte
    myPbHub.setPixelColor(CHAN_NewKEY, 0, 0, 255, 0);
    myPbHub.setPixelColor(CHAN_NewsKEY, 0, 0, 255, 0);
    myPbHub.setPixelColor(CHAN_NewssKEY, 0, 0, 255, 0);
  } else {
    myPbHub.setPixelColor(CHAN_KEY, 0, 255, 0, 0); // Séquence incorrecte, LED rouge
    myPbHub.setPixelColor(CHAN_NewKEY, 0, 255, 0, 0);
    myPbHub.setPixelColor(CHAN_NewsKEY, 0, 255, 0, 0);
    myPbHub.setPixelColor(CHAN_NewssKEY, 0, 255, 0, 0);
    delay(650);
    myPbHub.setPixelColor(CHAN_KEY, 0, 0, 0, 0);
    myPbHub.setPixelColor(CHAN_NewKEY, 0, 0, 0, 0);
    myPbHub.setPixelColor(CHAN_NewsKEY, 0, 0, 0, 0);
    myPbHub.setPixelColor(CHAN_NewssKEY, 0, 0, 0, 0);
    delay(450);
    myPbHub.setPixelColor(CHAN_KEY, 0, 255, 0, 0);
    myPbHub.setPixelColor(CHAN_NewKEY, 0, 255, 0, 0);
    myPbHub.setPixelColor(CHAN_NewsKEY, 0, 255, 0, 0);
    myPbHub.setPixelColor(CHAN_NewssKEY, 0, 255, 0, 0);
    delay(650);
    myPbHub.setPixelColor(CHAN_KEY, 0, 0, 0, 0);
    myPbHub.setPixelColor(CHAN_NewKEY, 0, 0, 0, 0);
    myPbHub.setPixelColor(CHAN_NewsKEY, 0, 0, 0, 0);
    myPbHub.setPixelColor(CHAN_NewssKEY, 0, 0, 0, 0);
    sequenceIndex = 0;
    currentIndex = 0;
    delay(160);
    hasExecuted = false;//RÉPÉTER LA SÉQUENCE SI RATÉ
  }
}
}}
