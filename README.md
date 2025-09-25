# EpiKodi

**EpiKodi** is a media center application developed as part of the Epitech simulated work experience during my 3rd year. This project aims to reproduce the core functionality of the Kodi media platform, demonstrating proficiency in C++17, Qt framework, and software architecture principles.

## Project Context

**Academic Framework**: Epitech Year 3 - Simulated Work Experience  
**Project Type**: Media Center Application  
**Inspiration**: Kodi Media Center Platform  
**Development Period**: 2025  
**Technologies**: C++17, Qt5/Qt6, CMake, GStreamer

## Project Objectives

This project demonstrates the following technical competencies:

### Software Architecture
- **Modular Design**: Separation between UI, media handling, and core logic
- **Interface Abstraction**: Multiple UI implementations (Qt GUI, Console)
- **Extensible Framework**: Easy addition of new features and components

### Media Processing
- **Multi-format Support**: Video, audio, and image file handling
- **Metadata Extraction**: File type detection and information display
- **Playback Integration**: Qt Multimedia framework integration

### User Experience
- **Intuitive Navigation**: Keyboard and mouse interaction patterns
- **Responsive Interface**: Adaptive layouts and error handling
- **Cross-platform Compatibility**: Linux/WSL support with future Windows expansion

## Technical Implementation

### Core Components
- **MediaFile**: File representation with type detection and metadata
- **FolderParser**: Recursive directory scanning and media discovery
- **Player**: Abstract playback interface with callback system
- **QtUI**: Full-featured GUI with media library and playback controls
- **ConsoleUI**: Terminal-based fallback interface

### Build System
- **CMake Configuration**: Multi-target builds with Qt version detection
- **Dependency Management**: Automated library detection and linking
- **Platform Support**: Linux/Ubuntu with WSL compatibility

## Documentation

Comprehensive documentation is available in the `docs/` folder:

- **[User Guide](docs/USER_GUIDE.md)**: Complete user manual with interface overview, navigation controls, and troubleshooting
- **[Technical Guide](docs/TECHNICAL_GUIDE.md)**: Developer documentation covering architecture, code structure, and implementation details

## Quick Start

### Prerequisites
```bash
# Ubuntu/Debian
sudo apt install build-essential cmake qtmultimedia5-dev \
    libqt5multimedia5-plugins gstreamer1.0-plugins-bad \
    gstreamer1.0-plugins-ugly gstreamer1.0-libav
```

### Build and Run
```bash
# Build the project
mkdir build && cd build
cmake ..
make -j$(nproc)

# Add media files
mkdir -p ../assets
# Place your videos, audio, and images in the assets/ folder

# Launch GUI version
./bin/epikodi

# Or launch console version
./bin/epikodi_console
```

## Supported Media Formats

| Type | Extensions | Features |
|------|------------|----------|
| **Video** | MP4, AVI, MKV, MOV, WMV, WebM | Full playback with controls |
| **Audio** | MP3, WAV, OGG, FLAC, AAC | Background playback |
| **Images** | JPG, PNG, GIF, BMP, TIFF | Direct viewing |

## Skills Demonstrated

### Programming Languages & Frameworks
- **C++17**: Modern C++ features, RAII, smart pointers
- **Qt5/Qt6**: GUI development, multimedia integration
- **CMake**: Build system configuration and dependency management

### Software Engineering
- **Design Patterns**: Observer pattern, Strategy pattern, Factory pattern
- **Error Handling**: Exception safety, graceful degradation
- **Testing**: Unit tests, integration testing strategies

### System Integration
- **Linux Development**: Filesystem APIs, process management
- **Multimedia Frameworks**: GStreamer integration, codec handling
- **Cross-platform Design**: Abstraction layers, platform-specific implementations

## Academic Context

This project fulfills the requirements of the Epitech simulated work experience by:

1. **Demonstrating Technical Proficiency**: Advanced C++ programming with modern frameworks
2. **Showing Architectural Thinking**: Clean, modular design with separation of concerns
3. **Implementing User-Centered Design**: Intuitive interfaces with comprehensive error handling
4. **Following Industry Standards**: Professional documentation, version control, and testing practices

## Future Enhancements

Planned improvements include:
- **Playlist Management**: Create and manage media playlists
- **Network Streaming**: Support for remote media sources
- **Plugin System**: Extensible architecture for third-party plugins
- **Advanced UI Themes**: Customizable interface themes and layouts

---

**Repository**: https://github.com/iiishuu/EpiKodi  
**Author**: Student at Epitech (Year 3)  
**Academic Year**: 2024-2025

*This project represents practical application of software engineering principles learned during the Epitech curriculum, with emphasis on modern C++ development and user interface design.*
