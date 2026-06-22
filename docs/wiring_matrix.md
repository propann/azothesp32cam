# Matrice de câblage validée — Azoth GlitchCam S3

## Règles de base
- Écran ST7796 et tactile FT6336U câblés sans utiliser GPIO0/GPIO46, qui
  déterminent le mode de démarrage de l'ESP32-S3.
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
| CTP_SCL | GPIO 41 | I2C | Horloge FT6336U ; GPIO46 interdit (strapping boot) |
| CTP_RST | 3V3 | I2C | Reset passif |
| CTP_INT | Non connecté | I2C | Non utilisé (polling) |
| SD_CS | Non connecté | SPI | Désactivé pour éviter conflits |

> Important : ne pas ajouter de pull-up ni de périphérique maintenant GPIO46
> à l'état haut. Le mode téléchargement UART exige GPIO0=LOW et GPIO46=LOW au
> moment où RESET est relâché.

## Caméra OV5640 AF
| Signal caméra | Broche ESP32-S3 |
|---|---:|
| SIOD | GPIO 4 |
| SIOC | GPIO 5 |
| XCLK | GPIO 15 |
| PCLK | GPIO 13 |
| VSYNC | GPIO 6 |
| HREF | GPIO 7 |
| D0 | GPIO 11 |
| D1 | GPIO 9 |
| D2 | GPIO 8 |
| D3 | GPIO 10 |
| D4 | GPIO 12 |
| D5 | GPIO 18 |
| D6 | GPIO 17 |
| D7 | GPIO 16 |
| PWDN | Non connecté / -1 |
| RESET | Non connecté / -1 |

## MicroSD native SDMMC 1-bit
| Signal SDMMC | Broche ESP32-S3 |
|---|---:|
| CLK | GPIO 39 |
| CMD | GPIO 38 |
| D0 | GPIO 40 |

## Encodeur rotatif KY-040
| Signal KY-040 | Broche ESP32-S3 | Note |
|---|---:|---|
| CLK / A | GPIO 1 | Entrée avec pull-up interne |
| DT / B | GPIO 2 | Entrée avec pull-up interne |
| SW | GPIO 42 | Bouton avec pull-up interne |
