# Modifications par rapport à OpenBoard 1.7.7

Base : OpenBoard, étiquette `v1.7.7` (https://github.com/OpenBoard-org/OpenBoard).

## Phase 1

- **Palette de 16 couleurs** (stylo et marqueur, fonds clair et foncé) au lieu de 5.
  Les 5 premières couleurs sont celles d'origine. Un seul bouton « Couleur » dans la barre d'outils
  ouvre une grille de 2 lignes x 8 colonnes. Les raccourcis des 5 couleurs d'origine fonctionnent toujours.
  Un ancien profil (palette de 5 couleurs) est automatiquement migré vers les 16 couleurs :
  la palette personnalisée éventuelle est remplacée.
- **Barre de menus classique**, toujours visible : Fichier, Édition, Page, Outils, Affichage, Aide.
  Elle réutilise les actions existantes (mêmes raccourcis, même état que les boutons de la barre d'outils).
  Le menu d'origine (bouton OpenBoard de la barre d'outils) reste en place.

Fichiers principaux : `src/gui/UBColorPaletteButton.*` (nouveau), `src/gui/UBMainWindow.cpp`,
`src/board/UBBoardController.cpp`, `src/core/UBSettings.cpp`, `src/core/UBSetting.*`,
`resources/etc/OpenBoard.config`, traductions `resources/i18n/OpenBoard_fr*.ts`.
