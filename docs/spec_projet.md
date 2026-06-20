# Spécification projet — Azoth GlitchCam S3

## Description
Azoth GlitchCam S3 est un firmware embarqué pour ESP32-S3 orienté création visuelle expérimentale.
Le système combine :
- une caméra OV5640,
- un écran TFT ST7796 3.5",
- un tactile FT6336U,
- un stockage microSD interne,
- un encodeur rotatif KY-040.

## Exigences fonctionnelles v1
- Démarrer les périphériques sans conflit de broches.
- Afficher une UI de statut claire.
- Détecter les touches et déclencher une action image.
- Capturer des frames caméra RGB565 QVGA et exposer leurs métadonnées.
- Appliquer des effets glitch directement sur le framebuffer RGB565.
- Sauvegarder les captures sur SD sous forme JPEG.
- Utiliser l'encodeur pour changer d'effet ou valider une action.
- Explorer les réglages capteur en live : luminosité, contraste, saturation, effet capteur et balance des blancs.

## Exigences techniques
- Framework Arduino via PlatformIO.
- Utilisation PSRAM obligatoire pour buffers caméra.
- Architecture modulaire maintenable (managers séparés).
- Documentation hardware/logicielle versionnée dans Git.
- Pinout unique aligné sur la carte ESP32-S3 WROOM N16R8.
- Effets RGB565 en place, sans allocation dynamique pendant la boucle vidéo.
