# Sääasema

Yhdistetään aiemmat osat sääasemaksi, joka näyttää näytöllä
verkosta haetun kellonajan sekä DHT11-sensorilta saadun lämpötilan
ja ilmankosteuden.

## Kirjastot

Kerätään ohjelman alkuun kaikki tarvittavat kirjastot ja globaalit muuttujat.

```c++
// Näyttö I2C-protokollalla
#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"
SSD1306Wire  display(0x3c, D3, D4);
// Näyttö SPI-protokollalla
// #include <SPI.h> // Only needed for Arduino 1.6.5 and earlier
// #include "SSD1306Spi.h"
// SSD1306Spi        display(D1, D2, D8);

// DHT11-sensori
#include "DHTesp.h"
DHTesp dht;

// ESP8266:n wifi-kirjasto
#include <ESP8266WiFi.h>

// Kirjastot ajan ja aikavyöhykkeiden käsittelyyn.
#include <time.h>
#include <TZ.h>

// WiFi-asetukset
#include "settings.h"
```

## Alustus

Funktiossa `setup()` tehdään jälleen käynnistyksen yhteydessä tehtävät alustukset.

### Näytön ja sensorin alustukset

Näyttö alustetaan `init()`-metodilla, asetetaan fontti, käännetään näyttö ylösalaisin,
asetetaan piirtoväri sekä kontrasti himmeäksi metodilla `setContrast()`.

Sensori asetetaan lukemaan oikeaa pinniä oikealla sensorin tyypillä.

### Verkkoyhteys ja NTP-aika

WiFi-verkkoon yhdistäminen tapahtuu funktiossa `connectNetwork()`. Funktioon on nyt
lisätty tulostukset näytölle.

NTP-kellonaika haetaan jälleen `getNTPTime()`-funktiossa samaan tapaan kuin esimerkissä
[05_ntp](../05_ntp/).


## Suoritus

Varsinainen suoritus tapahtuu taas `loop()`-funktion sisällä.
Luetaan sensorin mittaukset ja jos ne ovat kelvolliset, tulostetaan ne kellonajan sekä
päivämäärän ohessa näytölle funktioilla `drawTemperature()`, `drawHumidity()`
ja `drawPanel()`.