# Azoth GlitchCam S3

Firmware PlatformIO pour une base **solide et modulaire** de l'Azoth GlitchCam S3 (ESP32-S3 WROOM Cam + écran ST7796 3.5" + tactile FT6336U).

## Vision du projet
Construire un appareil photo glitch autonome qui :
- affiche une prévisualisation sur écran TFT,
- capture des images caméra,
- applique des effets visuels destructifs (glitch engine),
- sauvegarde les rendus sur la microSD interne de la carte.

## Câblage validé
Le câblage final est documenté ici :
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
└── include/
    ├── camera_manager.h
    ├── display_manager.h
    ├── touch_manager.h
    ├── storage_manager.h
    └── glitch_engine.h
```

## Démarrage rapide (PlatformIO)
1. Ouvrir ce dépôt dans VS Code + extension PlatformIO.
2. Compiler le firmware de validation écran+tactile.
3. Flasher la carte ESP32-S3.
4. Ouvrir le moniteur série à `115200`.

## État actuel
- ✅ Base de firmware de test écran/tactile opérationnelle.
- ✅ Paramétrage SPI 40MHz + I2C sur GPIO 48/46 conforme au plan matériel.
- ⏳ Modules caméra, stockage et glitch initialisés en squelettes prêts à implémenter.

## Prochaines étapes recommandées
1. Intégrer `camera_manager` (OV5640 + frame buffer PSRAM).
2. Intégrer `storage_manager` (SD interne + nommage fichiers robuste).
3. Créer une API de rendu entre `camera_manager`, `display_manager` et `glitch_engine`.
4. Ajouter des tests d'intégration et logs de performance.
