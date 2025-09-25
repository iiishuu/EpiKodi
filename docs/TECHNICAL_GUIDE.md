# Guide Technique EpiKodi

## Architecture

EpiKodi suit une architecture modulaire en 3 couches :

```
┌─────────────────┐
│   UI (Qt/Console) │  ← Interface utilisateur
├─────────────────┤
│   Core (App/Menu) │  ← Logique applicative
├─────────────────┤
│   Media (Files)   │  ← Gestion des fichiers
└─────────────────┘
```

### Composants principaux
- **MediaFile** : Représente un fichier avec détection de type
- **FolderParser** : Scanne les dossiers pour trouver les médias
- **Player** : Interface de lecture avec callbacks
- **QtUI** : Interface graphique Qt5
- **ConsoleUI** : Interface console de secours

## Structure du code

```
├── include/
│   ├── core/         # app.hpp, menu.hpp
│   ├── media/        # media_file.hpp, folder_parser.hpp, player.hpp
│   └── ui/           # qt_ui.hpp, console_ui.hpp
├── src/
│   ├── core/         # Implémentations core
│   ├── media/        # Gestion des fichiers média
│   ├── ui/           # Interfaces utilisateur
│   ├── main.cpp      # Point d'entrée GUI
│   └── main_console.cpp # Point d'entrée console
└── assets/           # Fichiers média (runtime)
```

## Composants détaillés

### MediaFile
```cpp
class MediaFile {
    enum class Type { VIDEO, AUDIO, IMAGE, UNKNOWN };
public:
    MediaFile(const std::string& path);
    Type getType() const;
    std::string getFormattedSize() const;
private:
    Type detectType() const;  // Détection par extension
};
```

### FolderParser
```cpp
class FolderParser {
public:
    FolderParser(const std::string& folderPath);
    std::vector<MediaFile> getMediaFiles();
private:
    void scanDirectory(const std::string& path);  // Scan récursif
};
```

### QtUI
Interface graphique avec :
- `QListWidget` pour la liste des fichiers
- `QVideoWidget` pour les vidéos
- `QLabel` pour les images
- Gestion clavier/souris

## Système de build

### CMake
```cmake
# Détection Qt5/Qt6 automatique
find_package(Qt5 QUIET COMPONENTS Core Widgets Multimedia)
if(NOT Qt5_FOUND)
    find_package(Qt6 REQUIRED COMPONENTS Core Widgets Multimedia)
endif()

# Deux exécutables
add_executable(epikodi src/main.cpp ...)           # GUI
add_executable(epikodi_console src/main_console.cpp ...) # Console
```

### Build
```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

## Conventions de code

### Nommage
- Classes : `PascalCase`
- Méthodes : `camelCase`  
- Membres privés : `name_`
- Constantes : `UPPER_SNAKE_CASE`

### Gestion d'erreurs
```cpp
// Retourner des optionals
std::optional<MediaFile> createMediaFile(const std::string& path);

// Messages d'erreur clairs pour l'utilisateur
void QtUI::showError(const std::string& message);
```

## Tests

### Tests unitaires simples
```cpp
void testMediaFileCreation() {
    MediaFile video("test.mp4");
    assert(video.getType() == MediaFile::Type::VIDEO);
}
```

### Exécution
```bash
cd build
./tests/test_media_file
```

## Support plateforme

- **Linux/Ubuntu** : Support complet avec Qt5 natif
- **WSL** : Support avec limitations audio/display
- **Futur** : Windows natif et macOS prévus