# WiFi

Opetellaan NodeMCU:n yhdistäminen WiFi-verkkoon.

Jotta NodeMCU:lla voidaan olla yhteydessä verkkoon, täytyy tehdä ainakin kaksi asiaa:
- Muodostaa yhteys WiFi-tukiasemaan.
- Tehdä yhteyspyyntöjä palvelimille.

## Kirjastot ja globaalit muuttujat

Ladataan ensimmäiseksi tarvittava WiFi-kirjasto:

```c++
#include <ESP8266WiFi.h>
```

Yhteyden luomiseksi tukiasemaan tarvitaan tukiaseman tarjoaman verkon nimi eli ssid
sekä sen salasana. Tässä esimerkkiohjelmassa nämä tiedot on laitettu erilliseen tiedostoon `settings.h`, joka ladataan mukaan komennolla:

```c++
#include "settings.h"
```

Tässä tiedostossa ssid ja salasana on määritelty merkkijonomuuttujina:
```c++
const char* ssid = "sssssssssss";
const char* password = "ppppppppppp";
```

Ennen kuin voit kääntää ja ajaa tätä ohjelmaa, sinun pitää luoda tuo tiedosto.
Kopioi pohjaksi annettu `settings.h.template` tälle nimelle ja vaihda sinne
käyttämäsi verkon tiedot.

Yhdistettävän palvelimen nimi on ohjelmassa laitettu muuttujaan `host`,
josta sitä on helppo käyttää muualla koodissa.
```c++
const char* host = "www.squix.org";
```

## Tukiasemaan yhdistäminen

Tukiasemaan yhdistämistä varten on tehty oma funktio `connectNetwork()`, jota
kutsutaan ohjelman `setup()`-alustusfunktiossa. Yhdistämisestä raportoidaan
joka vaiheessa sarjakonsoliin `Serial.print()`-komennoilla.

Ensin valitaan laitteen WiFi-piirin tila:
```c++
WiFi.mode(WIFI_STA);
```
Tilan arvo `WIFI_STA` tarkoittaa normaalia asiakastilaa. Muita vaihtoehtoja olisivat
`WIFI_AP` (tukiasematila), `WIFI_AP_STA` (tukiasema- ja asiakastila) sekä `WIFI_OFF` (wifi pois päältä).

Seuraavaksi aloitetaan tukiasemaan yhdistäminen:
```c++
WiFi.begin(ssid, password);
```

Ja odotetaan silmukassa, kunnes yhdistäminen on saatu valmiiksi. Lopuksi tulostetaan
sarjakonsoliin laitteelle saatu ip-osoite.


## Verkkosivun hakeminen

Kun alustustoimet, eli verkkoon kytkeytyminen, on tehty, voidaan palvelimelta pyytää
haluttu sivu. Tähän käytetään luokan `WiFiClient` objektia, joka esitellään komennolla:
```c++
WiFiClient client;
```

Palvelimelle haluttuun porttiin yhteyden ottaminen tapahtuu `connect()`-metodilla.

```c++
client.connect(host, httpPort);
```

Kun yhteys on saatu muodostettua, pyyntö halutusta sivusta lähetetään palvelimelle
`client.print()`-metodilla. Palvelimelle menevään viestiin siis "tulostetaan"
haluttu pyyntö http-protokollan mukaisena tekstinä.

Seuraavaksi odotetaan enintään 5 sekuntia vastausta palvelimelta. Jos vastaus
saapuu, se luetaan rivi kerrallaan ja tulostetaan sarjakonsoliin.

## Turvallisuus

Tämä esimerkkiohjelma käyttää yhteyden muodostamiseen salaamatonta http-protokollaa
porttiin 80. Tässä tapauksessa yhteys onnistuu, sillä esimerkkinä oleva palvelin
ottaa pyynnön vastaan. Nykyisin kuitenkin suuri osa palvelimista toimii
salatulla https-protokollalla portissa 443.

Tästä enemmän esimerkissä [04_wifi_https](../04_wifi_https/).