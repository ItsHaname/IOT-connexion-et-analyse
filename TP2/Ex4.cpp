#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11

#define LED_VERTE 8
#define LED_JAUNE  9
#define LED_ROUGE 10

DHT dht(DHTPIN, DHTTYPE);

unsigned long precedentMillis = 0;
const long intervalle = 500;
bool etatLED = false;

void eteindreToutesLesLEDs() {
  digitalWrite(LED_VERTE, LOW);
  digitalWrite(LED_JAUNE, LOW);
  digitalWrite(LED_ROUGE, LOW);
}

void faireClignoterRouge() {
  unsigned long actuel = millis();

  if (actuel - precedentMillis >= intervalle) {
    precedentMillis = actuel;
    etatLED = !etatLED;
    digitalWrite(LED_ROUGE, etatLED);
  }
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
  float humidite    = dht.readHumidity();

  if (isnan(temperature) || isnan(humidite)) {
    Serial.println("Erreur capteur");
    return;
  }

  eteindreToutesLesLEDs();

  // NORMAL : température < 25°C ET humidité < 70%
  if (temperature < 25 && humidite < 70) {
    digitalWrite(LED_VERTE, HIGH);
    Serial.println("ETAT NORMAL");
  }
  // ALERTE : température entre 25 et 30 OU humidité entre 70 et 80
  else if ((temperature >= 25 && temperature <= 30) ||
           (humidite >= 70 && humidite <= 80)) {
    digitalWrite(LED_JAUNE, HIGH);
    Serial.println("ETAT ALERTE");
  }
  // CRITIQUE : température > 30 OU humidité > 80
  else {
    faireClignoterRouge();
    Serial.println("ETAT CRITIQUE");
  }

  Serial.print("T = ");
  Serial.print(temperature);
  Serial.print(" °C | H = ");
  Serial.print(humidite);
  Serial.println(" %");
}

/*
 * ============================================================
 *  EXPLICATIONS — Système intelligent température + humidité
 * ============================================================
 *
 * 1. VARIABLES GLOBALES pour le clignotement
 *    ─────────────────────────────────────────
 *    unsigned long precedentMillis = 0
 *      Mémorise le dernier moment où la LED a changé d'état.
 *      Type unsigned long car millis() retourne une valeur
 *      pouvant dépasser 32 000 (max d'un int).
 *
 *    const long intervalle = 500
 *      Durée en ms entre chaque changement d'état de la LED.
 *      500 ms → la LED clignote 1 fois par seconde.
 *
 *    bool etatLED = false
 *      Mémorise si la LED rouge est allumée (true) ou
 *      éteinte (false). Commence éteinte.
 *
 * 2. faireClignoterRouge() — logique non-bloquante
 *    ─────────────────────────────────────────────
 *    millis() retourne le nombre de ms écoulées depuis
 *    le démarrage de l'Arduino (ne bloque pas le programme).
 *
 *    actuel - precedentMillis >= intervalle
 *      Vérifie si 500 ms se sont écoulées depuis le dernier
 *      changement. Si oui :
 *        - on met à jour precedentMillis = actuel
 *        - on inverse etatLED avec !etatLED (true↔false)
 *        - digitalWrite(LED_ROUGE, etatLED) allume ou éteint
 *
 *    ⚠️ Pourquoi ne pas utiliser delay(500) ?
 *    Avec delay(), l'Arduino se fige et ne peut plus lire
 *    le capteur. millis() permet un clignotement fluide
 *    SANS bloquer le reste du programme.
 *
 * 3. LOGIQUE DES TROIS ÉTATS
 *    ─────────────────────────
 *
 *    ┌──────────────────────────────────────────────────────┬──────────────┬──────────────┐
 *    │ Condition                                            │ LED          │ Message      │
 *    ├──────────────────────────────────────────────────────┼──────────────┼──────────────┤
 *    │ temp < 25 ET  humidite < 70                          │ VERTE fixe   │ ETAT NORMAL  │
 *    │ (25 ≤ temp ≤ 30) OU (70 ≤ humidite ≤ 80)            │ JAUNE fixe   │ ETAT ALERTE  │
 *    │ temp > 30  OU  humidite > 80                         │ ROUGE clign. │ ETAT CRITIQUE│
 *    └──────────────────────────────────────────────────────┴──────────────┴──────────────┘
 *
 * 4. OPÉRATEURS LOGIQUES utilisés
 *    ──────────────────────────────
 *    &&  (ET)  : les DEUX conditions doivent être vraies
 *    ||  (OU)  : au moins UNE condition doit être vraie
 *
 *    NORMAL   → && car les deux critères doivent être bons
 *    ALERTE   → || car un seul paramètre déjà préoccupant
 *    CRITIQUE → else (capturé automatiquement si ni NORMAL
 *               ni ALERTE)
 *
 * 5. eteindreToutesLesLEDs() en début de loop()
 *    ────────────────────────────────────────────
 *    Remet toutes les LEDs à LOW avant de décider laquelle
 *    allumer. Évite tout chevauchement d'états.
 *    Exception : en mode CRITIQUE, c'est faireClignoterRouge()
 *    qui gère elle-même l'état de la LED rouge.
 *
 * ============================================================
 *  SCHÉMA DE CÂBLAGE
 * ============================================================
 *
 *   DHT11     → Pin 2   (+ résistance 10kΩ vers 5V)
 *   LED Verte → Pin 8  → résistance 220Ω → GND
 *   LED Jaune → Pin 9  → résistance 220Ω → GND
 *   LED Rouge → Pin 10 → résistance 220Ω → GND
 *
 * ============================================================
 *  TABLEAU DES BLANCS REMPLIS
 * ============================================================
 *
 *   Ligne 25 : if ( actuel - precedentMillis >= intervalle )
 *   Ligne 28 : digitalWrite( LED_ROUGE , etatLED )
 *   Ligne 52 : if ( temperature < 25 && humidite < 70 )
 *   Ligne 56 : else if ( (temperature >= 25 && temperature <= 30)
 *                      || (humidite >= 70 && humidite <= 80) )
 *
 * ============================================================
 */
