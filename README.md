# Azoth GlitchCam S3

Firmware PlatformIO pour construire une **GlitchCam embarquée** basée sur ESP32-S3, avec une base de code robuste, modulaire et documentée en français.

## Description du projet
Azoth GlitchCam S3 vise à capturer des images depuis une OV5640, à appliquer des traitements glitch créatifs, puis à sauvegarder les résultats sur microSD tout en gardant une interface tactile locale sur écran ST7796.

## Matériel visé
- ESP32-S3 WROOM Cam
- Écran TFT ST7796 3.5"
- Tactile FT6336U (I2C)
- Caméra OV5640
- microSD (slot carte)
- Encodeur rotatif KY-040

## Câblage validé
Le schéma de référence figé est ici :
- [`docs/wiring_matrix.md`](docs/wiring_matrix.md)

## Architecture logicielle
```text
Azoth_GlitchCam/
├── platformio.ini
├── src/
│   ├── main.cpp
│   ├── camera_manager.cpp
│   ├── display_manager.cpp
│   ├── touch_manager.cpp
│   ├── storage_manager.cpp
│   ├── glitch_engine.cpp
│   └── encoder_manager.cpp
├── include/
│   ├── camera_manager.h
│   ├── display_manager.h
│   ├── touch_manager.h
│   ├── storage_manager.h
│   ├── glitch_engine.h
│   └── encoder_manager.h
└── docs/
    ├── wiring_matrix.md
    ├── spec_projet.md
    └── developpement_suivi.md
```

## Fonctionnement actuel (firmware de validation)
- Initialisation écran et rendu UI.
- Initialisation tactile sur GPIO 48/46.
- Initialisation caméra OV5640 AF en RGB565 QVGA.
- Initialisation SD_MMC.
- Preview temps réel avec glitch RGB565 en RAM.
- Sauvegarde JPEG sur microSD depuis le framebuffer RGB565.
- Contrôle tactile et encodeur KY-040.
- Exploration capteur en direct : luminosité, contraste, saturation, effets internes, balance des blancs.
- Effets glitch embarqués : RGB shift, corruption d'octets, scanline sort, miroir par bandes, solarisation, bruit, frame tear, masque de canaux.

## Limites actuelles
- Les effets glitch sont temps réel et sans tampon secondaire, donc certains sont volontairement abrasifs.
- Le dual-touch ajuste l'intensité, le menu tactile explore les effets et le capteur.
- L'autofocus OV5640 n'est pas encore piloté explicitement.

## Démarrage rapide
1. Installer VS Code + extension PlatformIO.
2. Ouvrir le dépôt.
3. Compiler l'environnement `azoth_esp32_s3_n16r8`.
4. Brancher le port USB du CH343, puis flasher la carte. PlatformIO detecte le
   port serie et esptool pilote automatiquement RESET/BOOT.
5. Ouvrir le moniteur série à `115200`.

Si la carte ne passe pas automatiquement en bootloader, maintenir `BOOT`,
appuyer brièvement sur `RESET`, relâcher `BOOT`, puis relancer l'upload. Une
erreur `No serial data received` signifie que la ROM de téléchargement ne
répond pas ; ce n'est ni une erreur de compilation ni un manque de flash.

Le projet utilise un board PlatformIO local :
- [`boards/azoth_esp32_s3_n16r8.json`](boards/azoth_esp32_s3_n16r8.json)

Il déclare explicitement ESP32-S3 WROOM, 16MB flash, 8MB PSRAM OPI, partitions 16MB et USB série externe CH343.

## Suivi et pilotage
- Spécification: [`docs/spec_projet.md`](docs/spec_projet.md)
- Journal de progression: [`docs/developpement_suivi.md`](docs/developpement_suivi.md)

## Vision court terme
1. Ajouter un écran de réglages complet pour l'encodeur et le tactile.
2. Ajouter des presets de style : VHS sale, scanner cassé, cyberpunk, noir et blanc brutal.
3. Ajouter un pilotage autofocus OV5640 et des profils caméra.
