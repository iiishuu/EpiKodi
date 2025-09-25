# EpiKodi v1.0

**EpiKodi** is a complete media center application developed as part of the Epitech simulated work experience during my 3rd year. This project successfully reproduces the core functionality of the Kodi media platform, demonstrating advanced proficiency in C++17, Qt framework, and professional software development practices.

## 🏆 Project Completion

**Academic Framework**: Epitech Year 3 - Simulated Work Experience  
**Project Status**: ✅ **COMPLETED** - Release v1.0  
**Development Period**: September 2025  
**Technologies**: C++17, Qt5/Qt6, CMake, GStreamer

## ✨ Implemented Features

### 🎵 **Audio & Video Playback**
- **MP3/WAV Audio Support**: Full audio playback with format validation
- **MP4 Video Playback**: Basic video playback with header verification  
- **Playback Controls**: Play, pause, stop, next with state management
- **Error Handling**: File corruption detection and format validation

### 📁 **Media Library Management**
- **Recursive Directory Scanning**: Automatic discovery of media files
- **Multi-format Support**: Video (MP4, AVI, MKV), Audio (MP3, WAV, OGG), Images (JPG, PNG, GIF)
- **Smart Type Detection**: Automatic file categorization with metadata display
- **File Information**: Size formatting, type indicators, and detailed info

### 🖥️ **Dual Interface System**
- **Qt GUI**: Full-featured graphical interface with media library and playback area
- **Console Mode**: Terminal-based fallback for environments without GUI support
- **Responsive Design**: Keyboard navigation and mouse interaction support
- **Professional UI**: Clean, modern interface with intuitive controls

### 🔧 **Advanced Architecture**
- **Modular Design**: Clean separation between UI, media handling, and core logic
- **Extensible Framework**: Easy addition of new features and components  
- **Cross-platform Support**: Linux/WSL native with Windows compatibility layer
- **Comprehensive Testing**: Unit tests for parsing and playback functionality

## 🚀 Quick Start

### Installation & Setup
```bash
# Install dependencies (Ubuntu/Debian)
sudo apt install build-essential cmake qtmultimedia5-dev \
    libqt5multimedia5-plugins gstreamer1.0-plugins-bad \
    gstreamer1.0-plugins-ugly gstreamer1.0-libav

# Clone and build
git clone https://github.com/iiishuu/EpiKodi.git
cd EpiKodi
mkdir build && cd build
cmake .. && make -j$(nproc)

# Add your media files
mkdir -p ../assets
# Place videos, audio, and images in the assets/ folder
```

### Usage
```bash
# Launch Qt GUI version
./bin/epikodi

# Launch console version  
./bin/epikodi_console

# Run tests
make tests
```

## 📊 Technical Achievement

| Component | Implementation Status | Features |
|-----------|----------------------|----------|
| **Media Parser** | ✅ Complete | Recursive scanning, type detection, metadata |
| **Audio Playback** | ✅ Complete | MP3/WAV support, format validation |
| **Video Playback** | ✅ Complete | MP4 support, header verification |
| **Player Controls** | ✅ Complete | Play/pause/stop/next, state management |
| **Error Handling** | ✅ Complete | File validation, corruption detection |
| **Qt GUI** | ✅ Complete | Full interface, keyboard/mouse navigation |
| **Console UI** | ✅ Complete | Terminal fallback, system integration |
| **Testing Suite** | ✅ Complete | Unit tests for all core functionality |

## 📋 Development Milestones

### ✅ Day 1: Foundation
- Project structure and CMake configuration
- Basic media file detection and parsing
- Core classes: MediaFile, FolderParser

### ✅ Day 2: Interface Development  
- Qt GUI implementation with media library
- Console interface for cross-platform compatibility
- Navigation and user interaction systems

### ✅ Day 3: Media Playback
- Player class with state management
- Audio/video format support and validation
- Playback controls and error handling
- File corruption detection

### ✅ Day 4: Polish & Release
- Comprehensive unit test suite
- Code documentation and cleanup  
- Final README and release preparation
- v1.0 tag and release notes

## 🧪 Testing & Quality Assurance

```bash
# Run all tests
make tests

# Individual test suites
make test-parser  # File detection and parsing
make test-player  # Media playback functionality
```

**Test Coverage:**
- ✅ Media file type detection
- ✅ Directory scanning and parsing  
- ✅ Audio/video playback functionality
- ✅ Player state management
- ✅ Error handling and validation

## 🎓 Skills Demonstrated

### **Technical Proficiency**
- **Modern C++17**: Smart pointers, RAII, standard library features
- **Qt Framework**: GUI development, multimedia integration, event handling
- **CMake**: Advanced build system configuration and cross-platform support
- **Software Architecture**: Clean design patterns, modular structure

### **Professional Development**
- **Version Control**: Git workflow with meaningful commit history
- **Testing**: Unit testing strategies and test-driven development
- **Documentation**: Comprehensive technical and user documentation
- **Project Management**: Milestone-based development with deliverables

### **System Integration**
- **Linux Development**: Filesystem APIs, process management, WSL compatibility
- **Multimedia Frameworks**: GStreamer integration, codec handling
- **Cross-platform Design**: Abstraction layers, platform-specific implementations

## 📦 Release v1.0

**Release Date**: September 25, 2025  
**Codename**: "Foundation"  

**What's New:**
- Complete media center functionality
- Dual interface system (GUI + Console)
- Full audio/video playback support
- Comprehensive testing suite
- Professional documentation

**Download**: [GitHub Releases](https://github.com/iiishuu/EpiKodi/releases/tag/v1.0)

---

**Repository**: https://github.com/iiishuu/EpiKodi  
**Author**: Student at Epitech (Year 3)  
**Academic Year**: 2024-2025

*This project represents the successful completion of a comprehensive media center application, demonstrating mastery of modern C++ development, GUI programming, and software engineering principles learned during the Epitech curriculum.*
