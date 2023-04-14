// Ladataan kirjasto. SSD1306 I2C:llä Wire:n avulla.
#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"

// Luodaan display-muuttuja ja kytketään se käyttämään pinnejä D3 ja D4.
// 0x3c on laitteen osoite
SSD1306Wire  display(0x3c, D3, D4);

// Jos käytössä on SPI-protokollaa käyttävä näyttö, käytetäänkin näitä rivejä.
// #include <SPI.h> // Only needed for Arduino 1.6.5 and earlier
// #include "SSD1306Spi.h"

// SSD1306Spi        display(D1, D2, D8);

// DHT11-sensorin ajurit
#include "DHTesp.h"
// Esitellään sensorin objekti.
DHTesp dht;

// ESP8266:n wifi-kirjasto
#include <ESP8266WiFi.h>

// Kirjastot ajan ja aikavyöhykkeiden käsittelyyn.
#include <time.h>
#include <TZ.h>

// Ladataan wifi:n ssid ja salasana erillisestä tiedostosta,
// ettei niitä tarvitse kirjoittaa tähän Githubiin menevän koodin sekaan.
// Käy lisäämässä ssid ja salasana ennen kuin yrität käyttää tätä.
#include "settings.h"

// ThingSpeak-palvelun käyttöön tarvittava kirjasto.
#include <ThingSpeak.h>
char thingSpeakAddress[] = "api.thingspeak.com";
unsigned long lastConnectionTime = 0;
long lastUpdateTime = 0;

WiFiClient client;

/**
 * Yhdistäminen tukiasemaan.
 **/
void connectNetwork() {
    Serial.print("Yhdistetään verkkoon ");
    Serial.println(ssid);
    // Tulostetaan tieto yhdistämisestä myös näyttöön.
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(10, 0, "Connecting to:");
    display.drawString(10, 16, ssid);
    display.display();
    
    // Asetetaan wifi client-tilaan.
    WiFi.mode(WIFI_STA);
    // Muodostetaan yhteys.
    WiFi.begin(ssid, password);
    
    // Odotetaan silmukassa, kunnes WiFi-yhteys on muodostunut.
    while (WiFi.status() != WL_CONNECTED) {
        // Puolen sekunnin odotus ja pisteen tulostaminen.
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("");
    Serial.println("WiFi yhdistetty");
    Serial.println("IP osoite: ");
    Serial.println(WiFi.localIP());
    // Näytetään ip-osoite näytöllä.
    display.drawString(10, 26, "WiFi Connected");
    display.drawString(10, 36, "IP Address:");
    display.drawString(10, 46, WiFi.localIP().toString());
    display.display();
    // Odotetaan pari sekuntia ennen kuin vaihdetaan näyttöön mittaustulokset.
    delay(2000);
}

/**
 * Noudetaan oikea aika NTP-palvelimelta.
 **/
void getNTPTime() {
    // Asetetaan aika NTP-palvelimelta.
    // Aikavyöhykkeiden nimet on määritelty TZ.h:ssa.
    // Nämä muuttujat sisältävät tiedon vyöhykkeestä ja kesäajasta.
    configTime(TZ_Europe_Helsinki, "pool.ntp.org", "time.nist.gov");
    
    Serial.print("Odotetaan NTP-ajan synkronointia: ");
    time_t now = time(nullptr);
    while (now < 8 * 3600 * 2) {
        delay(500);
        Serial.print(".");
        now = time(nullptr);
    }
    Serial.println("");
    struct tm timeinfo;
    gmtime_r(&now, &timeinfo);
    Serial.print("Current time: ");
    Serial.print(asctime(&timeinfo));
}

/**
 * Tulostetaan aika näytön yläosaan vasemmalle ja päivämäärä oikealle.
 **/
void drawPanel() {
    // https://cplusplus.com/reference/ctime/
    // https://cplusplus.com/reference/ctime/strftime/
    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    char tbuff[6];
    strftime(tbuff, 6, "%H.%M", timeinfo);
    display.drawString(0, 0, tbuff);
    // display.drawString(0, 0, String(timeinfo->tm_hour) + "." + String(timeinfo->tm_min));
    display.setTextAlignment(TEXT_ALIGN_RIGHT);
    char dbuff[15];
    strftime(dbuff, 15, "%a %d.%m.%Y", timeinfo);
    display.drawString(128, 0, dbuff);
}

/**
 * Tulostetaan lämpötila keskelle näyttöä isolla fontilla.
 **/
void drawTemperature(float temperature) {
    char tempb[4];
    char tbuff[6];
    sprintf(tbuff, "%s°C", dtostrf(temperature, 4, 1, tempb));
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_24);
    display.drawString(64, 20, tbuff);
}

/**
 * Tulostetaan ilmankosteus näytön oikeaan alakulmaan pienellä fontilla.
 **/
void drawHumidity(float humidity) {
    char humb[5];
    char hbuff[6];
    sprintf(hbuff, "%s %%", dtostrf(humidity, 4, 1, humb));
    display.setTextAlignment(TEXT_ALIGN_RIGHT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(128, 54, hbuff);
}

// Kirjoitetaan useampia kenttiä ThingSpeak-palveluun
// https://se.mathworks.com/help/thingspeak/read-and-post-temperature-data.html
int write2TSData( long TSChannel, unsigned int TSField1, float field1Data, unsigned int TSField2, float field2Data, unsigned int TSField3, long field3Data) {
    // Asetetaan kenttien arvot yksi kerrallaan.
    ThingSpeak.setField(TSField1, field1Data);
    ThingSpeak.setField(TSField2, field2Data);
    ThingSpeak.setField(TSField3, field3Data);
    // Kirjoitetaan asetetut arvot palvelimelle.
    int writeSuccess = ThingSpeak.writeFields( TSChannel, writeAPIKey );
    return writeSuccess;
}


void setup() {
    Serial.begin(115200);
    // Alustetaan näyttö.
    display.init();
    // Asetetaan fontin koko ja käännetään näyttö ylösalaisin.
    display.setFont(ArialMT_Plain_16);
    display.flipScreenVertically();
    // Asetetaan piirtoväri.
    display.setColor(WHITE);
    // Himmeä kirkkaus
    display.setContrast(10, 5, 0);
    // Normaali kirkkaus
    // display.setContrast(100);
    // DHT11-sensorin asetukset. (Pinni ja tyyppi);
    dht.setup(D6, DHTesp::DHT11);


    // Yhdistetään verkkoon.
    connectNetwork();
    // Asetetaan aika.
    getNTPTime();
}

void loop() {
    // Luetaan sensorilta lämpötila ja ilmankosteus.
    float temperature = dht.getTemperature();
    float humidity = dht.getHumidity();

    if (!isnan(humidity) && !isnan(temperature)) {
        // Jos kumpikaan ei ole NaN, tulostetaan ne näytölle.
        display.clear();

        // Tulostetaan mittaustulokset myös sarjakonsoliin printf()-funktiolla.
        Serial.printf("Hum: %.1f%%, Temp: %.1f°C\n", humidity, temperature);

        drawHumidity(humidity);
        drawTemperature(temperature);
        drawPanel();
        display.display();

        if (millis() - lastUpdateTime >= postingInterval) {
            // Jos edellisestä lähetyksestä on riittävästi aikaa, lähetetään ThingSpeakiin.
            lastUpdateTime = millis();
            int reply = write2TSData( channelID, dataFieldTemp, temperature, dataFieldHum, humidity, dataFieldTime, millis());
            // Tulostetaan sarjakonsoliin, debuggausta varten.
            Serial.print("ThingSpeak vastasi: ");
            Serial.println(reply);
        }
    } else {
        Serial.println("--- Tuli NaN");
    }

    // Wait
    delay(5000);

}

