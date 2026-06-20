# Suivi de développement — Azoth GlitchCam S3

## Objectif
Suivre l'avancement technique, les points validés, les risques, et les prochaines actions de manière exploitable dans Git.

## État actuel
- [x] Structure PlatformIO modulaire en place.
- [x] Pilote écran ST7796 (SPI) fonctionnel côté firmware.
- [x] Lecture tactile FT6336U (I2C polling) intégrée.
- [x] Initialisation caméra OV5640 ajoutée en RGB565 QVGA.
- [x] Initialisation SD_MMC intégrée.
- [x] Sauvegarde effective des captures JPEG déclenchée par UI.
- [x] Encodeur KY-040 intégré en polling.
- [x] Pipeline capture -> glitch -> rendu -> sauvegarde.
- [x] Effets glitch multiples en RGB565 sans tampon secondaire.
- [x] Exploration capteur live : luminosité, contraste, saturation, effets internes, balance des blancs.
- [ ] UI avancée de réglages et profils d'effets.
- [ ] Pilotage autofocus OV5640.

## Risques techniques
1. **Pinmap caméra OV5640 selon variante carte** : le mapping actuel suit le profil vérifié Freenove/ESP32-S3 WROOM mais doit rester documenté.
2. **GPIO encodeur** : GPIO 1, 2 et 42 sont exploités, validation électrique requise avec le câblage final.
3. **Débit SD_MMC** : la persistance de JPEG devra être profilée (latence + fragmentation).

## Plan court terme
1. Tester le firmware sur carte avec moniteur série.
2. Vérifier la direction de rotation KY-040 et inverser si nécessaire.
3. Ajouter des presets combinant effet glitch + réglage capteur.
4. Ajouter un pilotage autofocus OV5640.
