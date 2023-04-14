// Ladataan kirjastot näytön käyttämmistä varten.
// Käytä näitä rivejä, jos näyttö käyttää I2C-protokollaa.
#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"
// Käytä seuraavia rivejä, jos näyttö käyttää SPI-protokollaa.
// #include <SPI.h> // Only needed for Arduino 1.6.5 and earlier
// #include "SSD1306Spi.h"

// Alustetaan näyttöä ohjaava display-objekti.
// Tämä rivi alustaa I2C-protokollaa käyttävän näytön. 
SSD1306Wire  display(0x3c, D3, D4);  // Osoite ja liitäntäpinnit SDA, SCL
// Tämä rivi alustaa SPI-protokollaa käyttävän näytön.
// SSD1306Spi        display(D1, D2, D8);  // Liitäntäpinnit RES, DS, CS

// Sensorin ajurikirjasto.
#include "DHTesp.h"
// Esitellään ajuri.
DHTesp dht;

/**
 * Piirretään näytöllä lämpotila. Keskelle näyttöä isolla fontilla.
 **/
void drawTemperature(float temperature) {
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_24);

    display.drawString(64, 20, String(temperature) + "°C");
    // Vaihtoehtoiset rivit tulostuksen muotoiluun yhdellä desimaalilla.
    // char tempb[4];
    // char tbuff[6];
    // sprintf(tbuff, "%s°C", dtostrf(temperature, 4, 1, tempb));
    // display.drawString(64, 20, tbuff);
}

/**
 * Piirretään näytölle ilmankosteus. Oikeaan alakulmaan pienellä fontilla.
 **/
void drawHumidity(float humidity) {
    display.setTextAlignment(TEXT_ALIGN_RIGHT);
    display.setFont(ArialMT_Plain_10);

    display.drawString(128, 54, String(humidity) + " %");
    // Vaihtoehtoiset rivit tulostuksen muotoiluun yhdellä desimaalilla.
    // char humb[5];
    // char hbuff[6];
    // sprintf(hbuff, "%s %%", dtostrf(humidity, 4, 1, humb));
    // display.drawString(128, 54, hbuff);
}

/**
 * Alustus
 **/
void setup() {
    Serial.begin(115200);
    // Näytön alustus.
    display.init();

    // Asetetaan oletus fontti, käännetään näyttö.
    display.setFont(ArialMT_Plain_16);
    display.flipScreenVertically();

    // Asetetaan väri.
    display.setColor(WHITE);

    // Alustetaan sensorin ajuri. Datapinni ja sensorin tyyppi.
    dht.setup(D6, DHTesp::DHT11);
}

/**
 * Toistettava silmukka.
 **/
void loop() {
  

    // Luetaan sensorilta lämpötila ja ilmankosteus float-tyyppisiin muuttujiin.
    float temperature = dht.getTemperature();
    float humidity = dht.getHumidity();
    if (!isnan(humidity) && !isnan(temperature)) {
        // Datan lukeminen sensorilta saattaa epäonnistua jostain syystä.
        // (Virran saanti. Luetaan liian tiheään. Jokin muu syy.)
        // Silloin tuloksena on NaN (Not A Number), jota ei haluta tulostaa.
        // Tulostetaan näytölle vain, kun molemmat arvot ovat oikeita lukuja.
        display.clear();

        // Tulostetaan mittaustulokset myös sarjakonsoliin debuggausta varten.
        // Muotoillaan tulostus printf()-funktiolla. (yksi desimaali)
        Serial.printf("Hum: %.1f%%, Temp: %.1f°C\n", humidity, temperature);

        // Piirretään ilmankosteus ja lämpötila näyttömodulin muistiin.
        drawHumidity(humidity);
        drawTemperature(temperature);
        // Käsketään näytön piirtää muistissa oleva data näkyviin.
        display.display();
    } else {
        // Lämpötila, ilmankosteus tai molemmat oli NaN.
        // Tulostetaan sarjakonsoliin, että nähdään paljonko näitä tulee.
        Serial.println("--- Got nan");
    }

    // Odotetaan 10 s ennen seuraavaa kierrosta.
    delay(10000);

}

