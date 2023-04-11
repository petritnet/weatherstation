// Tiedoston alussa ladataan kirjastoja ja luodaan globaaleja muuttujia.
// Ladataan DHT-sensorien ajuri
#include "DHTesp.h"
// Esitellään uusi DHTesp-tyyppinen olio.
DHTesp dht;

/**
 * setup() -funktio suoritetaan kerran, kun NodeMCU käynnistyy.
 * Tässä funktiossa tehdään alustuksia yms.
 **/
void setup() {
    // Alustetaan Serial-olio nopeudella 115200 baud.
    // Serial:iin voi esimerkiksi tulostaa tekstiä.
    // Tools-valikosta voi avata "Serial Monitor" -näkymän, jossa näkyvät Serialiin tulostetut tekstit.
    Serial.begin(115200);

    // Alustetaan dht-olio.
    // Sen data-johto on kytketty NodeMCU:n pinniin D6.
    // Sen tyyppi on DHT11.
    dht.setup(D6, DHTesp::DHT11);
}

/**
 * loop() -funktiota suoritetaan toistuvasti loputtomasti, kun setup() -funktio on suoritettu.
 **/
void loop() {
  
    // Luetaan lämpötila ja ilmankosteus sensorista float-tyyppisiin muuttujiin.
    float temperature = dht.getTemperature();
    float humidity = dht.getHumidity();
    // Kirjoitetaan Serial-olion printf -metodilla arvot yhden desimaalin tarkkuuteen muotoiltuina sarjakonsoliin.
    Serial.printf("Hum: %.1f%%, Temp: %.1f°C\n", humidity, temperature);

    // Odotetaan 4 sekuntia ennen loop() -funktion seuraavaa suoritusta.
    delay(4000);

}

