# Guide Utilisateur EpiKodi

## Démarrage rapide

### Premier lancement
1. **Ajouter vos fichiers média** : Placez vos vidéos, musiques et images dans le dossier `assets/`
2. **Lancer EpiKodi** :
   - Interface graphique : `./build/bin/epikodi`
   - Console : `./build/bin/epikodi_console`
3. **Explorer** : Vos fichiers sont automatiquement détectés

### Prérequis
- Linux/Ubuntu ou WSL
- Qt5 et codecs installés

## Interface graphique (Qt)

### Navigation
- **Flèches ↑↓** : Parcourir la bibliothèque
- **Entrée** ou **Double-clic** : Lire/afficher le fichier sélectionné
- **Espace** : Lecture/Pause (vidéo/audio)
- **Échap** : Arrêter la lecture

### Affichage
- **Panneau gauche** : Liste des fichiers média avec icônes de type
- **Panneau droit** : Lecteur vidéo ou visionneuse d'images
- **Barre d'état** : Informations sur le fichier (type, taille)

## Interface console

### Commandes
- **1-9** : Sélectionner et lire un média
- **l** ou **list** : Actualiser la liste
- **h** ou **help** : Afficher l'aide
- **q** ou **quit** : Quitter

## Formats supportés

| Type | Extensions | 
|------|------------|
| **Vidéo** | MP4, AVI, MKV, MOV, WMV |
| **Audio** | MP3, WAV, OGG, FLAC, AAC |
| **Images** | JPG, PNG, GIF, BMP |

## Problèmes courants

### "Aucun fichier trouvé"
- Vérifiez que vos fichiers sont dans le dossier `assets/`
- Formats supportés uniquement

### Pas de son/vidéo
```bash
sudo apt install ubuntu-restricted-extras
```

### Interface graphique ne s'ouvre pas (WSL)
```bash
export DISPLAY=:0
```