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
│   └── glitch_engine.cpp
├── include/
│   ├── camera_manager.h
│   ├── display_manager.h
│   ├── touch_manager.h
│   ├── storage_manager.h
│   └── glitch_engine.h
└── docs/
    ├── wiring_matrix.md
    ├── spec_projet.md
    └── developpement_suivi.md
```

## Fonctionnement actuel (firmware de validation)
- Initialisation écran et rendu UI.
- Initialisation tactile sur GPIO 48/46.
- Initialisation caméra (avec capture de diagnostic au toucher).
- Initialisation SD_MMC.
- Logs série détaillés au boot et pendant les captures.

## Limites actuelles
- Le mapping OV5640 peut varier selon la variante exacte de carte ESP32-S3 CAM.
- La sauvegarde JPEG est prête côté API (`StorageManager`) mais pas encore branchée dans le flux UI.
- Le moteur de glitch est un squelette à compléter.

## Démarrage rapide
1. Installer VS Code + extension PlatformIO.
2. Ouvrir le dépôt.
3. Compiler l'environnement `esp32-s3-devkitc-1`.
4. Flasher la carte.
5. Ouvrir le moniteur série à `115200`.

## Suivi et pilotage
- Spécification: [`docs/spec_projet.md`](docs/spec_projet.md)
- Journal de progression: [`docs/developpement_suivi.md`](docs/developpement_suivi.md)

## Vision court terme
1. Valider pinmap caméra réelle (et corriger `camera_manager`).
2. Ajouter capture + sauvegarde JPEG sur action tactile.
3. Ajouter preview caméra + premiers effets glitch temps réel.
