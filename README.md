# 🎬 EpiKodi - Media Center

A modern media center application inspired by Kodi, built with Qt5/C++17.
*Simplified reimplementation of Kodi (C++ media center project for B-PRO-500)*

## 📋 Features

### ✨ Core Functionality
- **🎥 Multi-format Media Support**: Video (MP4, AVI, MKV, MOV, WMV), Audio (MP3, WAV, OGG, FLAC), Images (JPG, PNG, GIF, BMP)
- **🖼️ Smart Media Handling**: Automatic type detection with appropriate display/playback
- **📁 Directory Scanning**: Recursive media file discovery with intelligent filtering
- **🎯 Dual Interface**: Qt GUI and console fallback for different environments

### 🎮 User Interface
- **Modern Qt5 GUI**: Dark theme with intuitive media center styling
- **📱 Responsive Layout**: Adaptive interface with media library sidebar and preview area
- **⌨️ Keyboard Navigation**: Full keyboard control (arrows, enter, space, escape)
- **🖱️ Mouse Support**: Click and double-click interactions
- **📊 Rich Media Info**: File type indicators, size formatting, and detailed metadata

### 🔧 Technical Features
- **🏗️ Clean Architecture**: Separation of UI, media handling, and core logic
- **🔄 Cross-platform**: Linux/WSL support with Windows compatibility layer
- **🎯 Error Handling**: Comprehensive error messages and graceful fallbacks
- **📦 Modular Design**: Pluggable UI components (Qt, Console)

## 🚀 Quick Start

### Prerequisites
- **C++17** compatible compiler
- **Qt5** development libraries
- **CMake 3.16+**

#### Ubuntu/Debian Installation
```bash
sudo apt update
sudo apt install -y build-essential cmake qtmultimedia5-dev \
    libqt5multimedia5-plugins qt5-qmake qtbase5-dev \
    gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly \
    gstreamer1.0-libav ubuntu-restricted-extras
```

### Build & Run
```bash
# Clone and navigate
git clone https://github.com/iiishuu/EpiKodi.git
cd EpiKodi

# Build
mkdir build && cd build
cmake ..
make -j$(nproc)

# Add your media files
mkdir -p ../assets
# Place your videos, audio, and images in the assets/ folder

# Run Qt GUI version
./bin/epikodi

# Or run console version
./bin/epikodi_console
```

## 🎯 Usage

### Qt GUI Version
- **Navigation**: Use ⬆️⬇️ arrow keys or mouse to browse media
- **Play/View**: Press `Enter` or double-click to play/view selected media
- **Playback Control**: 
  - `Space`: Play/Pause (video/audio)
  - `Escape`: Stop playback
- **Media Info**: Detailed information displayed in the sidebar

### Console Version
- **Select Media**: Type the number (1-9) to launch media with system player
- **Commands**: 
  - `l` or `list`: Show media library
  - `h` or `help`: Display help
  - `q` or `quit`: Exit application

## 📁 Project Structure

```
EpiKodi/
├── 📂 assets/                 # Media files directory
├── 📂 include/               # Header files
│   ├── 📂 core/              # Core application logic
│   ├── 📂 media/             # Media handling components
│   └── 📂 ui/                # User interface components
├── 📂 src/                   # Source files
│   ├── 📂 core/              # App, Menu implementations
│   ├── 📂 media/             # MediaFile, Player, FolderParser
│   ├── 📂 ui/                # QtUI, ConsoleUI implementations
│   ├── 📄 main.cpp           # Qt GUI entry point
│   └── 📄 main_console.cpp   # Console entry point
├── 📂 build/                 # Build artifacts
├── 📂 docs/                  # Documentation
├── 📂 tests/                 # Unit tests
└── 📄 CMakeLists.txt         # Build configuration
```

## 🎨 Interface

### Qt GUI Interface
- **Media Library**: Clean sidebar with file type icons and metadata
- **Playback Area**: Dedicated video player with image viewer
- **Controls**: Intuitive media controls and status information

### Console Interface
- **File Browser**: Numbered list with media type and size information
- **System Integration**: Launches files with appropriate system applications

## 🔧 Development

### Architecture Overview
- **Core**: Application logic and media processing
- **Media**: File handling, parsing, and metadata extraction
- **UI**: Modular interface system supporting multiple frontends

### Supported Formats
| Type  | Extensions | Description |
|-------|------------|-------------|
| 🎥 Video | .mp4, .avi, .mkv, .mov, .wmv, .flv, .webm, .m4v | Full playback support |
| 🎵 Audio | .mp3, .wav, .ogg, .flac, .aac, .m4a, .wma | Audio playback |
| 🖼️ Images | .jpg, .jpeg, .png, .gif, .bmp, .tiff, .webp | Direct display |

## 🐛 Troubleshooting

### WSL/Linux Issues
- **Audio crackling**: Normal in WSL environment due to virtualization
- **No GUI display**: Ensure `DISPLAY` variable is set (`export DISPLAY=:0`)
- **Missing codecs**: Install `ubuntu-restricted-extras` package

## 📝 License

This project is part of the Epitech curriculum (2025).

---
*Built with ❤️ using Qt5, C++17, and CMake*
