// ESP8266:n wifi-kirjasto
#include <ESP8266WiFi.h>
// Kirjasto suojattua https-yhteyttä varten.
// Vastaava kuin oletuksena mukana oleva WiFiClient
#include <WiFiClientSecure.h>

// Ladataan wifi:n ssid ja salasana erillisestä tiedostosta,
// ettei niitä tarvitse kirjoittaa tähän Githubiin menevän koodin sekaan.
// Käy lisäämässä ssid ja salasana ennen kuin yrität käyttää tätä.
#include "settings.h"

// Palvelin, johon otetaan testiyhteys.
const char* host = "wttr.in";

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


void setup() {
    Serial.begin(115200);

    // Yhdistetään verkkoon.
    connectNetwork();
}

void loop() {

    delay(5000);

    Serial.print("Yhdistetään palvelimelle ");
    Serial.println(host);

    // WiFiClientSecure-luokkaa voi käyttää TCP-yhteyteen.
    WiFiClientSecure client;
    const int httpPort = 443;
    // Käytetään palvelimelta wttr.in etukäteen ladattua julkista avainta sertifikaatin varmistamiseen.
    // Julkinen avain on kopioitu settings.h-tiedostoon muuttujaksi pubkey.
    BearSSL::PublicKey key(pubkey);
    client.setKnownKey(&key);
    // Turvattomampi vaihtoehto on jättää palvelimen sertifikaatti tarkistamatta!
    // client.setInsecure();

    if (!client.connect(host, httpPort)) {
        // Jos yhteys ei onnistunut, tulostetaan.
        Serial.println("Yhteys epäonnistui!");
        return;
    }

    // Haettavan sivun osoite on http://wttr.in/turku?2nTA
    String url = "/turku?2nTA";

    Serial.print("Haetaan osoitteesta: ");
    Serial.println(url);

    // Seuraava lähettää palvelimelle sivupyynnön.
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
        "Host: " + host + "\r\n" +
        "Connection: closse\r\n\r\n");
    
    // Lakataan odottamasta vastausta, jos sitä ei kuulu 5 sekunnin aikana.
    unsigned long timeout = millis();
    while (client.available() == 0) {
        // Vastausta ei ole vielä tullut.
        if (millis() - timeout > 5000) {
            // Aikaa on kulunut yli 5 s.
            Serial.println(">>> Aikakatkaisu!");
            client.stop();
            return;
        }
    }

    // Kun vastaus on tullut, luetaan sitä rivi kerrallaan ja tulostetaan, kunnes loppuu.
    while (client.available()) {
        String line = client.readStringUntil('\r');
        Serial.print(line);
    }

    delay(20000);

}

