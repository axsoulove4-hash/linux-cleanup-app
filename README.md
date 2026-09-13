# Linux Cleanup / ClariSweep

Application C++20 et Qt6, interface vectorielle inspirée du modèle fourni.

Port Windows : voir [guide Windows](windows/README-Windows.md). Sources adaptées et script de compilation/déploiement fournis. Build et tests Linux validés ; EXE Windows pas encore compilé ni testé.

## Lancer

```bash
/home/alexisf/clarisweep/build/clarisweep
```

## Comportement

- Analyse asynchrone automatique par défaut. Distribution et noyau lus sur machine.
- Tableau de bord : octets réellement mesurés, diagramme proportionnel. Aucun chiffre illustratif.
- Nettoyage : liste détaillée avec case par fichier. Suppression définitive des fichiers cochés après confirmation explicite, caches, miniatures, logs, corbeille et temporaires compris. Fichiers modifiés depuis analyse, redirections et fichiers hors catégorie ignorés. Résultat : nombre et taille supprimés, ignorés, échecs.
- Applications : tuiles avec logos des lanceurs locaux, système et Flatpak, recherche et classement alphabétique. Double-clic pour informations. Icône générique quand aucun logo disponible.
- Stockage : cartes avec graphique circulaire, nom et espace occupé/disponible. Montages techniques, boucles et partitions de démarrage masqués ; montages d'un même périphérique regroupés.
- Éléments volumineux : noms, icônes, tailles, dossiers abrégés, recherche et filtres images/vidéos/documents. Jusqu'à 500 fichiers au-dessus du seuil, triés par taille, ouverture du dossier.
- Paramètres persistants : Français / English, clair/sombre, accent, analyse au démarrage, ancienneté minimum, seuil des gros fichiers, dossier et exclusions. Choisir langue puis Enregistrer et réanalyser ; interface actualisée sans relancer l'application. Noms propres et messages bruts du gestionnaire de paquets restent inchangés.

## Périmètre

Caches utilisateur, miniatures, logs .log sous XDG_STATE_HOME, corbeille utilisateur et fichiers /tmp appartenant à l'utilisateur. Par défaut, seuls caches, miniatures, logs et temporaires de plus de 14 jours comptent. Corbeille mesurée sans limite d'âge. Liens symboliques exclus. Nombre d'éléments inaccessibles indiqué. Tailles logiques : peuvent différer de l'espace physique libérable (compression, liens physiques, snapshots).

Paquets : suggestions en lecture seule avec DNF (cache local), APT ou pacman ; aucun volume inventé, aucune désinstallation automatique. Aucun diagnostic artificiel de santé du système.

## Compiler et vérifier

```bash
cmake -S /home/alexisf/clarisweep -B /home/alexisf/clarisweep/build
cmake --build /home/alexisf/clarisweep/build -j4
ctest --test-dir /home/alexisf/clarisweep/build --output-on-failure
QT_QPA_PLATFORM=offscreen /home/alexisf/clarisweep/build/clarisweep --verify
```

Vérification : captures build/page-0.png à page-5.png et rapport build/verification.json. Tests de suppression limités à des fichiers créés dans un dossier temporaire de test ; aucune donnée utilisateur nettoyée par les tests.
