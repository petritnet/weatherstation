// Ladataan kirjasto. SSD1306 I2C:llä Wire:n avulla.
#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"

// Luodaan display-muuttuja ja kytketään se käyttämään pinnejä D3 ja D4.
// 0x3c on laitteen osoite
SSD1306Wire  display(0x3c, D3, D4);

// Ladataan mukaan kuvia tiedostosta images.h.
#include "images.h"

// Demojen näyttöaika. 5 s
#define DEMO_DURATION 5000
// Määritellään Demo-tyyppi pointterina.
typedef void (*Demo)(void);
// Näytettävän demon järjestysnumero
int demoNumber = 0;
// Lasketaan suoritetut loop()-kierrokset.
int counter = 1;

// Tekstin näyttäminen
void drawText() {
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 0, "Hei maailma!");
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_16);
    display.drawString(64, 16, "Hei maailma!");
    display.setTextAlignment(TEXT_ALIGN_RIGHT);
    display.setFont(ArialMT_Plain_24);
    display.drawString(128, 32, "Heippa!");
}

// Kuvioiden piirtämistä
void drawFigures() {
  // Ympyrä
  display.drawCircle(16, 16, 14); // keskipiste ja säde
  // Suorakaide
  display.drawRect(80, 2, 40, 30); // Vasen yläkulma x, y, leveys, korkeus
  // Täytetty suorakaide
  display.fillRect(4, 32, 40, 20);
  // Viivoja
  display.drawHorizontalLine(70, 55, 40); // Alkupiste ja pituus
  display.drawVerticalLine(110, 30, 25); // Alkupiste ja pituus
  // Pisteitä
  for (int i = 0; i < 10; i++) {
    display.setPixel(110 - i, 55 - i);
  }
}

// Kuvien piirtäminen
void drawImages() {
    // Kuvien data on tiedostossa images.h. Katso!
    // Xbm-tiedostomuoto on bittikarttakuva C++-koodina.
    // Xbm-kuvia voi tallettaa esimerkiksi Gimp-ohjelmalla. ("Vie nimellä...")
    display.drawXbm(0, 0, black_cat1_width, black_cat1_height, black_cat1_bits);
    display.drawXbm(50, 20, heart_width, heart_height, heart_bits);
    display.drawXbm(70, 2, black_cat2_width, black_cat2_height, black_cat2_bits);
}

// Edistymispalkki
void drawProgressBar() {
    int progress = (counter / 5) % 100;
    // Piirretään edistymispalkki
    // Vasen yläkulma, oikea alakulma, edistyminen välillä 0...100
    display.drawProgressBar(0, 32, 120, 10, progress);
}


void setup() {
    Serial.begin(115200);

    // Alustetaan näyttö.
    display.init();

    // Asetetaan fontti. Oletuksena käytettävissä: ArialMT_Plain_10, ArialMT_Plain_16, ArialMT_Plain_24
    display.setFont(ArialMT_Plain_16);
    // Jos näyttö tarvitsee kääntää ylösalaisin, käytetään tätä.
    // display.flipScreenVertically();

    // Asetetaan piirtoväri.
    display.setColor(WHITE);
}

// Luettelo demoista
Demo demos[] = {drawText, drawFigures, drawImages, drawProgressBar};
// Demojen määrä
int demoLength = (sizeof(demos) / sizeof(Demo));
long timeSinceDemoSwitch = 0;

void loop() {
    // Putsataan näyttö.
    display.clear();
    // Piirretään demo
    demos[demoNumber]();
    // Kirjoitetaan laskuri näkyviin.
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0, 54, String(counter));
    // Päivitetään näyttö tehdyillä piirroksilla.
    display.display();

    // Jos edellisestä vaihdosta on riittävästi aikaa,
    // vaihdetaan seuraavaan demonumeroon.
    if (millis() - timeSinceDemoSwitch > DEMO_DURATION) {
        demoNumber = (demoNumber +1) % demoLength;
        timeSinceDemoSwitch = millis();
    }

    counter++;

    // Odotetaan vain vähän.
    delay(10);
}

