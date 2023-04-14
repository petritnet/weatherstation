// ESP8266:n wifi-kirjasto
#include <ESP8266WiFi.h>

// Kirjastot ajan ja aikavyöhykkeiden käsittelyyn.
#include <time.h>
#include <TZ.h>

// Ladataan wifi:n ssid ja salasana erillisestä tiedostosta,
// ettei niitä tarvitse kirjoittaa tähän Githubiin menevän koodin sekaan.
// Käy lisäämässä ssid ja salasana ennen kuin yrität käyttää tätä.
#include "settings.h"

/**
 * Yhdistäminen tukiasemaan.
 **/
void connectNetwork() {
    Serial.print("Yhdistetään verkkoon ");
    Serial.println(ssid);
    
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
 * Tulostetaan aika sarjakonsoliin.
 **/
void printTime() {
    // https://cplusplus.com/reference/ctime/
    // https://cplusplus.com/reference/ctime/strftime/
    // Otetaan raaka aika laitteen kellosta ja muodostetaan
    // siitä paikallista aikaa oleva tm-tyyppinen muuttuja timeinfo.
    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    // Muodostetaan 6 merkkiä pitkä merkkijono, jossa on kellonaika. (tunnit ja minuutit)
    char tbuff[6];
    strftime(tbuff, 6, "%H.%M", timeinfo);
    // Muodostetaan 15 merkkiä pitkä merkkijono, jossa on päivämäärä. (viikonpäivä, päivä, kuukausi, vuosi)
    char dbuff[15];
    strftime(dbuff, 15, "%a %d.%m.%Y", timeinfo);
    // Tulostetaan sarjakonsoliin.
    Serial.println(tbuff);
    Serial.println(dbuff);
}

void setup() {
    Serial.begin(115200);

    // Yhdistetään verkkoon.
    connectNetwork();
    // Asetetaan aika.
    getNTPTime();
}

void loop() {

    printTime();

    delay(1000);

}

