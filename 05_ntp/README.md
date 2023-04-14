# NTP-aika

Kun NodeMCU on yhteydessä Internetiin, sieltä voidaan pyytää oikea aika NTP-palvelimelta.

Tämä ohjelma sisältää WiFi-tukiasemaan yhdistämiseksi saman koodin kuin edelliset
osat [04_wifi](../04_wifi/) ja [04_wifi_https](../04_wifi_https/).

## Kirjastot

Uudet kirjastot, jotka tässä ohjelmassa ladataan, ovat:
```c++
#include <time.h>
#include <TZ.h>
```

Näistä ensimmäinen sisältää yleisesti ajan käsittelyyn tarvittavia tietotyyppejä.
Jälkimmäisessä on määritelty eri aikavyöhykkeiden asetukset helpommin muistettavina
niminä. Nämä asetukset sisältävät varsinaisen aikavyöhykkeen sekä tiedon kesäajan
käytöstä. Suomea koskevan aikavyöhykkeen nimi on `TZ_Europe_Helsinki`.

Myös tässä ohjelmassa ladataan WiFi-tukiaseman asetukset erillisestä tiedostosta
`settings.h`. Muista luoda tämä tiedosto kopioimalla mallina oleva `settings.h.template`
ja lisäämällä sinne käyttämäsi tukiaseman tiedot.

```c++
#include "settings.h"
```

## NTP-ajan noutaminen

Uusi toiminto tässä ohjelmassa on alustuksessa `setup()` kutsuttava
funktio `getNTPTime()`.

Aika asetetaan komennolla
```c++
configTime(TZ_Europe_Helsinki, "pool.ntp.org", "time.nist.gov");
```

Tässä ensimmäisenä parametrina on halutun aikavyöhykkeen tiedot ja kaksi muuta ovat
NTP-palvelimet, joilta aika haetaan.

Funktion `while`-silmukassa odotetaan, kunnes aika on synkronoitunut laitteelle.
Lopuksi tulostetaan aika.

## Ajan tulostaminen

Toinen uusi funktio on `printTime()`, jossa pyydetään laitteelta ensin
`time()`-funktiolla `time_t`-tyyppinen raaka aika, sitten muodostetaan tästä
funktiolla `localtime()` paikallinen aika, joka on tyyppiä `tm`.
Lopuksi `strftime()` funktiolla muodostetaan merkkijonot, jotka kuvaavat
kellonaikaa ja päivämäärää, sekä tulostetaan nämä molemmat sarjakonsoliin.

Lisää C++-kielen ajan käsittelystä `time.h`-kirjastolla voi opiskella osoitteesta
https://cplusplus.com/reference/ctime/ .
Päivämäärän muotoilemisesta `strftime()`-funktiolla voi puolestaan lukea lisää osoiteesta
https://cplusplus.com/reference/ctime/strftime/ .

