#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  float temperature = dht.readTemperature();
  float humidite = dht.readHumidity();

  if (isnan(temperature) || isnan(humidite)) {
    Serial.println("Erreur de lecture du capteur DHT11");
  } else {
    Serial.print("Temperature : ");
    Serial.print(temperature);
    Serial.print(" °C | Humidite : ");
    Serial.print(humidite);
    Serial.println(" %");
  }

  delay(2000);
}

/*
 * ============================================================
 *  EXPLICATIONS DU CODE — Capteur DHT11 avec Arduino
 * ============================================================
 *
 * 1. #include <DHT.h>
 *    Inclut la bibliothèque DHT d'Adafruit qui fournit les
 *    fonctions nécessaires pour communiquer avec le capteur.
 *
 * 2. #define DHTPIN 2
 *    Définit la broche numérique 2 de l'Arduino comme broche
 *    de données du capteur DHT11.
 *
 * 3. #define DHTTYPE DHT11
 *    Précise le modèle du capteur utilisé (DHT11).
 *    Peut aussi être DHT22 ou DHT21 selon le matériel.
 *
 * 4. DHT dht(DHTPIN, DHTTYPE);
 *    Crée un objet "dht" en lui passant la broche et le type.
 *    Cet objet sera utilisé pour toutes les lectures.
 *
 * 5. setup()
 *    - Serial.begin(9600) : initialise la communication série
 *      à 9600 bauds pour afficher les données dans le moniteur.
 *    - dht.begin() : initialise le capteur DHT11.
 *
 * 6. dht.readTemperature()
 *    Lit la température en degrés Celsius depuis le capteur.
 *    Retourne NaN (Not a Number) si la lecture échoue.
 *
 * 7. dht.readHumidity()
 *    Lit le taux d'humidité relative (en %) depuis le capteur.
 *    Retourne NaN si la lecture échoue.
 *
 * 8. isnan(temperature) || isnan(humidite)
 *    Vérifie si l'une des deux valeurs est invalide (NaN).
 *    isnan() = "is Not a Number" → retourne true si la valeur
 *    n'est pas un nombre valide (capteur absent ou défaillant).
 *    Le || signifie "OU" : une seule erreur suffit à déclencher
 *    le message d'erreur.
 *
 * 9. Serial.print() / Serial.println()
 *    Envoie les valeurs vers le moniteur série de l'IDE Arduino.
 *    println() ajoute un saut de ligne à la fin.
 *
 * 10. delay(2000)
 *     Attend 2000 ms (2 secondes) entre chaque lecture.
 *     Le DHT11 ne peut pas être lu plus d'une fois par seconde,
 *     donc un délai minimum de 1000 ms est recommandé.
 *
 * ============================================================
 *  SCHÉMA DE CÂBLAGE DU DHT11
 * ============================================================
 *
 *   DHT11          Arduino Uno
 *   ------         -----------
 *   VCC    ------> 5V
 *   DATA   ------> Pin 2  (+ résistance pull-up 10kΩ vers 5V)
 *   GND    ------> GND
 *
 * ============================================================
 */
