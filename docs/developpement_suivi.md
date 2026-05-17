# Suivi de développement — Azoth GlitchCam S3

## Objectif
Suivre l'avancement technique, les points validés, les risques, et les prochaines actions de manière exploitable dans Git.

## État actuel
- [x] Structure PlatformIO modulaire en place.
- [x] Pilote écran ST7796 (SPI) fonctionnel côté firmware.
- [x] Lecture tactile FT6336U (I2C polling) intégrée.
- [x] Initialisation caméra OV5640 ajoutée avec capture de frame de diagnostic.
- [x] Initialisation SD_MMC intégrée.
- [ ] Sauvegarde effective des captures JPEG déclenchée par UI.
- [ ] Pipeline complet capture -> glitch -> rendu -> sauvegarde.

## Risques techniques
1. **Pinmap caméra OV5640 selon variante carte** : le mapping actuel est une base et doit être validé matériellement.
2. **Conflits GPIO potentiels** : certains modules S3 CAM réassignent des pins caméra/PSRAM différemment.
3. **Débit SD_MMC** : la persistance de JPEG devra être profilée (latence + fragmentation).

## Plan court terme
1. Valider le pinmap caméra réel de la carte et corriger `camera_manager.cpp`.
2. Ajouter `capture_and_save()` avec nommage horodaté et compteur fallback.
3. Afficher les erreurs détaillées `esp_err_t` sur écran + série.
4. Ajouter un mode preview caméra (downscale) dans la zone 320x180.
