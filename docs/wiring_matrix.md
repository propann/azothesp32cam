# Matrice de câblage validée — Azoth GlitchCam S3

## Règles de base
- Écran ST7796 et tactile FT6336U câblés uniquement sur des GPIO libres de l'ESP32-S3 WROOM Cam.
- MicroSD de l'écran non utilisée (slot SD de la carte S3 uniquement).
- `LCD_CS` tiré à GND et `LCD_RST` tiré à 3V3 pour économiser des GPIO.

## Tableau des connexions
| Broche Écran / Tactile | Broche ESP32-S3 | Type de bus | Rôle / note |
|---|---:|---|---|
| VCC | 5V | Alim | Alimentation dalle + logique |
| GND | GND | Alim | Masse commune |
| LCD_CS | GND | SPI | Sélection permanente (pas de pin CS logicielle) |
| LCD_RST | 3V3 | SPI | Reset matériel passif |
| LCD_RS / DC | GPIO 3 | SPI | Data/Command |
| SDI / MOSI | GPIO 47 | SPI | Données écran |
| SCK | GPIO 21 | SPI | Horloge SPI (40 MHz) |
| LED | 3V3 | Alim | Rétroéclairage toujours actif |
| SDO / MISO | Non connecté | SPI | Inutile (écriture seule) |
| CTP_SDA | GPIO 48 | I2C | Données FT6336U |
| CTP_SCL | GPIO 46 | I2C | Horloge FT6336U |
| CTP_RST | 3V3 | I2C | Reset passif |
| CTP_INT | Non connecté | I2C | Non utilisé (polling) |
| SD_CS | Non connecté | SPI | Désactivé pour éviter conflits |
