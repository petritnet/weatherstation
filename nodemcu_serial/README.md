# Sarjasyöte ThingSpeakiin

Tämä NodeMCU-piirillä ajettava koodi lukee pinneihin D2 (read) ja D3 (transmit)
kytketystä laitteesta (Micro:bit) lähetettyä sarjadataa ja lähettää sen ThingSpeak-palveluun.

## Kytkentä

| Micro:bit | NodeMCU |
|-----------|---------|
| tx        | D2      |
| rx        | D3      |
| gnd       | Gnd     |
| vcc       | 3.3V    |

(Virrat kytketään yhteen, jos laitteista toinen syöttää virtaa toiseen. Tarvittavan volttimäärän mukaan.)

## Asetukset

Verkon asetukset ja ThingSpeakin avaimet pitää laittaa tiedostoon `settings.h`.
Kopioi pohjana oleva tiedosto `settings.h.template` tälle nimelle ja
täytä sinne oikeat tiedot.
```c++
// Vaihdan WiFi:n tiedot tähän.
const char* ssid = "sssssssss";
const char* password = "ppppppppp";

// ThingSpeak
// Vaihda tähän ThinkSpeakin kanavan tiedot ja avaimet.
unsigned long channelID = <kanavan numero>;
const char* writeAPIKey = "<kirjoitus api-avain>";
const char* readAPIKey = "<luku api-avain>";

// ThingSpeakin asetuksia päivitystiheys
const unsigned long postingInterval = 120L * 1000L; // 2 minutes
```

## Datan muoto

Sarjadata tulee tekstiriveinä muodossa, jossa on pilkuilla eroteltuina
ensin rivin otsikko ja sitten lukuarvoja. Esimerkiksi:
```
"test",25.88,1000.48,131.74,32.47
```

## Datan lukeminen

Funktiossa `loop()` luetaan sarjadata rivi kerrallaan ja tulkitaan siitä arvot.
Ensin poistetaan alusta ensimmäisenä oleva teksti ja sen jälkeen luetaan kukin
lukuarvo muuttujaan. Esimerkiksi:
```c++
// Parse 1st value
token = strtok(NULL, ",");
float val1 = atof(token);
```

Muuttujia luetaan enintää 8 kpl. Ota kommentointi pois riittävästä määrästä rivejä.

## Datan lähettäminen

Luetut datat lisätään kukin kenttä `ThingSpeak`-olioon riveillä:
```c++
ThingSpeak.setField(1, val1); // Kenttä 1
```

Ota kommentointi pois riittävästä määrästä rivejä.

Lisätyt kentät lähetetään ThingSpeak-palveluun rivillä:
```c++
int writeSuccess = ThingSpeak.writeFields( channelID, writeAPIKey );
```
