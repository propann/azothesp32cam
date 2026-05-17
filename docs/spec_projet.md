# Spécification projet — Azoth GlitchCam S3

## Description
Azoth GlitchCam S3 est un firmware embarqué pour ESP32-S3 orienté création visuelle expérimentale.
Le système combine :
- une caméra OV5640,
- un écran TFT ST7796 3.5",
- un tactile FT6336U,
- un stockage microSD interne.

## Exigences fonctionnelles v1
- Démarrer les périphériques sans conflit de broches.
- Afficher une UI de statut claire.
- Détecter les touches et déclencher une action image.
- Capturer des frames caméra et exposer leurs métadonnées.
- Préparer la chaîne de sauvegarde SD.

## Exigences techniques
- Framework Arduino via PlatformIO.
- Utilisation PSRAM obligatoire pour buffers caméra.
- Architecture modulaire maintenable (managers séparés).
- Documentation hardware/logicielle versionnée dans Git.
