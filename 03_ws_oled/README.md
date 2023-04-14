# Sääasema näytöllä

Yhdistetaan DHT11:n sensorien käyttö ja OLED-näytön käyttö.

Luetaan lämpötila ja ilmankosteus DHT11-sensorilta ja näytetään ne OLED-näytöllä.

Tässä ohjelmassa on yhdistetty kahden ensimmäisen osan tiedot. Uusia kirjastoja ei tarvitse asentaa tätä ohjelmaa varten.

Aluksi ladataan tarvittavat kirjastot näyttöä ja sensoria varten sekä alustetaan kumpaakin varten globaali objekti vastaavasti kuin edellisissäkin vaiheissa. Huomaa, että näytön käyttöä varten pitää valita joko I2C- tai SPI-protokollaa vastaavien ajureiden kirjastot.

Lämpötilan ja ilmankosteuden piirtäminen näytölle on kumpikin kirjoitettu omaksi funktiokseen, jotka saavat yhden parametrin, `float` tyyppisen muuttujan, joka on kyseinen mittaustulos. Näissä molemmissa funktioissa mittaustulos kirjoitetaan ruudulle muuntamalla mittauksen numeerinen arvo `String` tyyppiseksi ja kirjoittamalla yhteen (katenoimalla) mittayksikön kanssa. Esim.

```c++
display.drawString(64, 20, String(temperature) + "°C");
```

Kannattaa huomata, että tämä tulostaa mittaustuloksen kahdella desimaalilla, joista jälkimmäisenä oleva 0 on oikeastaan turha. Jos haluaa kokeilla katkaista tulostuksen yhden desimaalin kohdalta, voi kokeilla vaihtaa tulostuskomennon tilalle kommentoituna olevat rivit, jotka muotoilevat tulosteen ennen piirtämistä näytölle. Esim.
```c++
char tempb[4];
char tbuff[6];
sprintf(tbuff, "%s°C", dtostrf(temperature, 4, 1, tempb));
display.drawString(64, 20, tbuff);
```

Ensin `setup()`-funktiossa tehdään tarvittavat kertaluonteiset alustustoimet.
Toistettavan `loop()`-funktion sisällä mittausarvot luetaan sensorilta ja tulostetaan näytölle. Sensorin mittausarvon lukeminen voi jossain tilanteessa epäonnistua. Silloin sensorilta saatava arvo on `NaN`, eli "Not A Number". Tätä ei haluta tulostaa näytölle. Tämän takia näytön päivittäminen on laitettu `if`-lauseen sisälle. Ehdossa tarkistetaan, ettei kumpikaan mittaustuloksista ole `NaN` ja päivitetään näyttöä vain tällöin.

Lisäksi koodissa tulostetaan ohjelmoinnin virheidenmetsästyksen vuoksi myös sarjakonsoliin.

Silmukkafunktion lopussa odotetaan `10000` millisekuntia, eli 10 sekuntia, ennen seuraavaa mittauskierrosta. Tämä riittänee näytteenottotiheydeksi. Tätä voi säätää tarpeen mukaan.