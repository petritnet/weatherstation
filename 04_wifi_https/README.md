# WiFi (https)

Opetellaan NodeMCU:n yhdistäminen WiFi-verkkoon ja verkon käyttö salatulla
https-protokollalla.

Jotta NodeMCU:lla voidaan olla yhteydessä verkkoon, täytyy tehdä ainakin kaksi asiaa:
- Muodostaa yhteys WiFi-tukiasemaan.
- Tehdä yhteyspyyntöjä palvelimille.

## Kirjastot ja globaalit muuttujat

Ladataan ensimmäiseksi tarvittava WiFi-kirjasto:

```c++
#include <ESP8266WiFi.h>
```

Tämän lisäksi ladataan suojattuun yhteyteen tarvittava kirjasto:
```c++
#include <WiFiClientSecure.h>
```
Tällä saadaan käyttöön luokka `WiFiClientSecure`, joka vastaa esimerkissä
[04_wifi](../04_wifi/) käytettyä luokkaa `WiFiClient`, mutta sisältää
tuen salaukselle ja palvelimen varmenteen (sertifikaatin) tarkistamiselle.

Yhteyden luomiseksi tukiasemaan tarvitaan tukiaseman tarjoaman verkon nimi eli ssid
sekä sen salasana. Tässä esimerkkiohjelmassa nämä tiedot on laitettu erilliseen tiedostoon `settings.h`, joka ladataan mukaan komennolla:

```c++
#include "settings.h"
```

Tässä tiedostossa ssid ja salasana on määritelty merkkijonomuuttujina.
```c++
const char* ssid = "sssssssssss";
const char* password = "ppppppppppp";
```

Lisäksi tiedostossa on upotettuna palvelimen wttr.in varmenteen julkinen
avain, jotta sen varmenne voidaan tarkistaa yhteyttä luotaessa.
Normaalisti selaimilla on tiedossa varmenteiden myöntäjien avaimet ja yksittäisten
palvelinten varmenteet voidaan tarkistaa sitä kautta. Yhteen palvelimeen yhteydessä
olevan pienen sulautetun laitteen tapauksessa voi kuitenkin olla yksinkertaisempaa
vain sisällyttää kyseisen palvelimen julkinen avain mukaan koodiin.

Ennen kuin voit kääntää ja ajaa tätä ohjelmaa, sinun pitää luoda tuo tiedosto.
Kopioi pohjaksi annettu `settings.h.template` tälle nimelle ja vaihda sinne
käyttämäsi verkon tiedot.

Yhdistettävän palvelimen nimi on ohjelmassa laitettu muuttujaan `host`,
josta sitä on helppo käyttää muualla koodissa.
```c++
const char* host = "wttr.in";
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
haluttu sivu. Tähän käytetään nyt luokan `WiFiClientSecure` objektia, joka esitellään komennolla:
```c++
WiFiClientSecure client;
```

Erona suojaamattomaan yhteyteen on käytetty luokka, yhteyden portti (443) sekä
komennot
```c++
BearSSL::PublicKey key(pubkey);
client.setKnownKey(&key);
```
joilla ensin muodostetaan `BearSSL::PublicKey`-tyyppiä oleva julkinen avain
ja asetetaan se `client`-objektille tunnetuksi avaimeksi. Nyt yhteyttä luotaessa
`client` osaa tunnistaa palvelimen varmenteen ja suostuu muodostamaan yhteyden.
Toinen vaihtoehto olisi kytkeä varmenteen tarkistaminen pois päältä komennolla
`client.setInsecure()`. Tämä tarkoittaisi, että yhteys kyllä salattaisiin,
mutta ei varmennettaisi, että keskukstellaan oikean palvelimen kanssa.

Palvelimelle haluttuun porttiin yhteyden ottaminen tapahtuu `connect()`-metodilla.

```c++
client.connect(host, httpPort);
```

Kun yhteys on saatu muodostettua, pyyntö halutusta sivusta lähetetään palvelimelle
`client.print()`-metodilla. Palvelimelle menevään viestiin siis "tulostetaan"
haluttu pyyntö http-protokollan mukaisena tekstinä.

Seuraavaksi odotetaan enintään 5 sekuntia vastausta palvelimelta. Jos vastaus
saapuu, se luetaan rivi kerrallaan ja tulostetaan sarjakonsoliin.

Vastauksena sarjakonsoliin pitäisi tulostua samanlainen sääennuste kuin
menemällä selaimella osoitteeseen https://wttr.in/turku?2nTA .
