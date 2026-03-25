#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11

#define LED_VERTE 8
#define LED_JAUNE  9
#define LED_ROUGE 10

DHT dht(DHTPIN, DHTTYPE);

void eteindreToutesLesLEDs() {
  digitalWrite(LED_VERTE, LOW);
  digitalWrite(LED_JAUNE, LOW);
  digitalWrite(LED_ROUGE, LOW);
}

void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(LED_VERTE, OUTPUT);
  pinMode(LED_JAUNE, OUTPUT);
  pinMode(LED_ROUGE, OUTPUT);
}

void loop() {
  float temperature = dht.readTemperature();

  if (isnan(temperature)) {
    Serial.println("Erreur de lecture");
    return;
  }

  eteindreToutesLesLEDs();

  if (temperature < 22) {
    digitalWrite(LED_VERTE, HIGH);
  }
  else if (temperature >= 22 && temperature <= 28) {
    digitalWrite(LED_JAUNE, HIGH);
  }
  else {
    digitalWrite(LED_ROUGE, HIGH);
  }

  Serial.print("Temperature : ");
  Serial.println(temperature);

  delay(2000);
}

/*
 * ============================================================
 *  EXPLICATIONS — Système à trois niveaux de température
 * ============================================================
 *
 * 1. #define LED_VERTE 8 / LED_JAUNE 9 / LED_ROUGE 10
 *    Trois LEDs sur trois broches distinctes.
 *    Chaque couleur représente une plage de température.
 *
 * 2. eteindreToutesLesLEDs()
 *    Fonction utilitaire appelée au début de chaque loop().
 *    Elle met les 3 broches à LOW avant d'en rallumer une seule.
 *    Cela garantit qu'une seule LED est active à la fois,
 *    sans risque de chevauchement entre deux états.
 *    C'est une bonne pratique : "reset puis set".
 *
 * 3. Structure if / else if / else
 *
 *    ┌─────────────────────────┬────────────┐
 *    │ Condition               │ LED active │
 *    ├─────────────────────────┼────────────┤
 *    │ temperature < 22°C      │ VERTE  🟢  │
 *    │ 22°C ≤ temp ≤ 28°C      │ JAUNE  🟡  │
 *    │ temperature > 28°C      │ ROUGE  🔴  │
 *    └─────────────────────────┴────────────┘
 *
 *    Le else final capture automatiquement temperature > 28
 *    car les deux conditions précédentes ont déjà filtré
 *    les cas < 22 et entre 22 et 28.
 *
 * 4. Pourquoi écrire temperature >= 22 && temperature <= 28 ?
 *    Le && (ET logique) impose que les DEUX conditions soient
 *    vraies simultanément. Cela définit un intervalle fermé
 *    [22 ; 28] incluant les bornes.
 *
 * 5. digitalWrite(LED_X, HIGH)
 *    Après avoir tout éteint, on allume uniquement la LED
 *    correspondant à la plage détectée.
 *    HIGH = 5V → courant dans la LED → elle s'allume.
 *
 * 6. delay(2000)
 *    Pause de 2 secondes. Le DHT11 nécessite au moins
 *    1 seconde entre deux lectures pour être fiable.
 *
 * ============================================================
 *  SCHÉMA DE CÂBLAGE
 * ============================================================
 *
 *   DHT11  → Pin 2  (+ résistance 10kΩ vers 5V)
 *   LED Verte → Pin 8  → résistance 220Ω → GND
 *   LED Jaune → Pin 9  → résistance 220Ω → GND
 *   LED Rouge → Pin 10 → résistance 220Ω → GND
 *
 * ============================================================
 *  TABLEAU DES BLANCS REMPLIS
 * ============================================================
 *
 *   Ligne 37 : if ( temperature < 22 )
 *   Ligne 38 : digitalWrite( LED_VERTE , HIGH )
 *   Ligne 40 : else if ( temperature >= 22 && temperature <= 28 )
 *   Ligne 41 : digitalWrite( LED_JAUNE , HIGH )
 *   Ligne 44 : digitalWrite( LED_ROUGE , HIGH )
 *
 * ============================================================
 */
