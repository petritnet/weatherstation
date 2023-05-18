# Petrin sääaseman datat

Tähän hakemistoon on noudettu ThingSpeak-palvelusta Petrin sääaseman
keräämää dataa. Kussakin csv-tiedostossa on yhden viikon verran mittauksia.
Yksi sarja mittaustuloksia kullakin rivillä.
Tiedostonimi viittaa aikajakson päätöshetkeen kello 0:00:00 kyseisenä päivänä (UTC).

Jupyter-notebook-tiedostossa csv-tiedostot ladataan ja niistä piirretään kuvaaja.

## Datan kentät

1. `created_at`: Tietueen aikaleima, jolloin mittaustulokset on mitattu. UTC-aikana.
2. `entry_id`: ThingSpeakin tietueelle antama id kyseisellä kanavalla. Tietueen järjestysnumero.
3. `field1`: **Lämpötila** (°C) (*temperature*)
4. `field2`: **Suhteellinen kosteus** (%) (*humidity*)
5. `field3`: Uptime, eli kauanko sääasema on kyseisellä hetkellä ollut yhtäjaksoisesti päällä. (ms)