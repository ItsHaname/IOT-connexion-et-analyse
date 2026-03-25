#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11

#define LED_VERTE 8
#define LED_ROUGE 9

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(LED_VERTE, OUTPUT);
  pinMode(LED_ROUGE, OUTPUT);
}

void loop() {
  float temperature = dht.readTemperature();

  if (isnan(temperature)) {
    Serial.println("Erreur capteur");
    return;
  }

  if (temperature < 25) {
    digitalWrite(LED_VERTE, HIGH);
    digitalWrite(LED_ROUGE, LOW);
  } else {
    digitalWrite(LED_VERTE, LOW);
    digitalWrite(LED_ROUGE, HIGH);
  }

  Serial.print("Temperature : ");
  Serial.println(temperature);

  delay(2000);
}

/*
 * ============================================================
 *  EXPLICATIONS — Indicateur thermique à deux LEDs
 * ============================================================
 *
 * 1. #define LED_VERTE 8 / LED_ROUGE 9
 *    Associe un nom symbolique à chaque broche.
 *    La LED verte est connectée à la broche 8,
 *    la LED rouge à la broche 9.
 *    Utiliser des #define rend le code plus lisible et
 *    plus facile à modifier si on change de broche.
 *
 * 2. pinMode(LED_VERTE, OUTPUT)
 *    pinMode(LED_ROUGE, OUTPUT)
 *    Déclare les deux broches comme sorties numériques.
 *    Sans cela, les broches sont en mode INPUT par défaut
 *    et ne peuvent pas alimenter les LEDs.
 *
 * 3. dht.readTemperature()
 *    Lit la température en °C depuis le capteur DHT11.
 *    Si le capteur est absent ou défaillant, retourne NaN.
 *
 * 4. isnan(temperature)
 *    Vérifie si la valeur lue est invalide (Not a Number).
 *    Si c'est le cas, on affiche une erreur et on quitte
 *    le loop() immédiatement avec return, sans toucher
 *    aux LEDs (elles gardent leur état précédent).
 *
 * 5. if (temperature < 25)
 *    Condition principale du programme :
 *    - temperature < 25°C  → ambiance normale/fraîche
 *                          → LED VERTE allumée
 *    - temperature >= 25°C → température élevée
 *                          → LED ROUGE allumée
 *
 * 6. digitalWrite(LED_VERTE, HIGH) + digitalWrite(LED_ROUGE, LOW)
 *    HIGH = 5V → la LED reçoit du courant → elle s'allume.
 *    LOW  = 0V → pas de courant          → elle s'éteint.
 *    On commande toujours les DEUX LEDs simultanément pour
 *    garantir qu'une seule reste active à la fois.
 *    (Sinon les deux pourraient être allumées au démarrage.)
 *
 * 7. delay(2000)
 *    Pause de 2 secondes entre chaque lecture.
 *    Le DHT11 nécessite au minimum 1 seconde entre deux
 *    mesures pour garantir des données fiables.
 *
/ * ============================================================
 *  SCHÉMA DE CÂBLAGE
 * ============================================================
 *
 *   DHT11          Arduino
 *   ------         -------
 *   VCC    ------> 5V
 *   DATA   ------> Pin 2  (+ résistance 10kΩ vers 5V)
 *   GND    ------> GND
 *
 *   LED Verte : Pin 8 → résistance 220Ω → anode LED → cathode → GND
 *   LED Rouge : Pin 9 → résistance 220Ω → anode LED → cathode → GND
 *
 * ============================================================
 *  TABLEAU DES BLANCS REMPLIS
 * ============================================================
 *
 *   Ligne 15 : pinMode( LED_VERTE , OUTPUT )
 *   Ligne 16 : pinMode( LED_ROUGE , OUTPUT )
 *   Ligne 27 : if ( temperature < 25 )
 *   Ligne 28 : digitalWrite( LED_VERTE , HIGH )
 *   Ligne 29 : digitalWrite( LED_ROUGE , LOW  )
 *   Ligne 31 : digital */
