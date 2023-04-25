#include <SoftwareSerial.h> // Tarvitsee kirjaston ESPSoftwareSerial
#include <ESP8266WiFi.h>

// Luodaan sarjaobjekti. Kytketään luku pinniin D2 ja kirjoitus pinniin D3.
SoftwareSerial s(D2, D3);

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
    // Odotetaan pari sekuntia ennen kuin vaihdetaan näyttöön mittaustulokset.
    delay(2000);
}

void setup() {
    Serial.begin(9600);
    s.begin(4800);

    // Asetetaan D2 input-tilaan ja D3 output-tilaan.
    pinMode(D2, INPUT);
    pinMode(D3, OUTPUT);
    
    // Yhdistetään verkkoon.
    connectNetwork();

    // Alustetaan ThingSpeak verkkoyhteydelä.
    ThingSpeak.begin(client);
}

void loop() {
    while (s.available() > 0) {
        String text = s.readStringUntil('\n');
        Serial.println(text);
        
        char* token;
        // Data in format: 26.20,1033.00,-138.16,22.85
        // Parse the text
        token = strtok((char *)text.c_str(), ",");
        float check = atof(token);

        // Parse 1st value
        token = strtok(NULL, ",");
        float val1 = atof(token);
        
        // Parse 2nd value
        token = strtok(NULL, ",");
        float val2 = atof(token);
        
        // Parse 3rd value
        token = strtok(NULL, ",");
        float val3 = atof(token);
        
        // Parse 4th value
        token = strtok(NULL, ",");
        float val4 = atof(token);
        
        // // Parse 5th value
        // token = strtok(NULL, ",");
        // float val5 = atof(token);
        
        // // Parse 6th value
        // token = strtok(NULL, ",");
        // float val6 = atof(token);
        
        // // Parse 7th value
        // token = strtok(NULL, ",");
        // float val7 = atof(token);
        
        // // Parse 8th value
        // token = strtok(NULL, ",");
        // float val8 = atof(token);
        
        // Tulostetaan debuggausta varten.
        Serial.println("Value 1: " + String(val1));
        Serial.println("Value 2: " + String(val2));
        Serial.println("Value 3: " + String(val3));
        Serial.println("Value 4: " + String(val4));
        // Serial.println("Value 5: " + String(val5));
        // Serial.println("Value 6: " + String(val6));
        // Serial.println("Value 7: " + String(val7));
        // Serial.println("Value 8: " + String(val8));
        Serial.println();
        Serial.println();
        if (lastUpdateTime == 0 || millis() - lastUpdateTime >= postingInterval) {
            // Jos edellisestä lähetyksestä on riittävästi aikaa, lähetetään ThingSpeakiin.
            lastUpdateTime = millis();
            // Kirjoitetaan useampia kenttiä ThingSpeak-palveluun
            // https://se.mathworks.com/help/thingspeak/read-and-post-temperature-data.html
            ThingSpeak.setField(1, val1); // Kenttä 1
            ThingSpeak.setField(2, val2); // Kenttä 2
            ThingSpeak.setField(3, val3);
            ThingSpeak.setField(4, val4);
            // ThingSpeak.setField(5, val5);
            // ThingSpeak.setField(6, val6);
            // ThingSpeak.setField(7, val7);
            // ThingSpeak.setField(8, val8);
            ThingSpeak.setField(5, long(millis())); // Laitetaan uptime kenttään 5, jos ei se oli muuten käytössä.

            // Lähetetään data
            int writeSuccess = ThingSpeak.writeFields( channelID, writeAPIKey );
            
            // Tulostetaan sarjakonsoliin debuggausta varten.
            Serial.print("--------- ThingSpeak vastasi: ");
            Serial.println(writeSuccess);
            Serial.println();
        }
    }
    delay(5000);
}
