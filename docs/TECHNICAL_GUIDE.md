# EpiKodi Technical Documentation

This document provides detailed technical information for developers working on or extending EpiKodi.

## Table of Contents
1. [Architecture Overview](#architecture-overview)
2. [Code Structure](#code-structure)
3. [Core Components](#core-components)
4. [Build System](#build-system)
5. [Development Guidelines](#development-guidelines)
6. [Testing](#testing)
7. [Platform Support](#platform-support)

## Architecture Overview

EpiKodi follows a modular, layered architecture that separates concerns and enables easy extension:

```
┌─────────────────────────────────────────┐
│                UI Layer                 │
│  ┌─────────────┐    ┌─────────────┐    │
│  │   Qt GUI    │    │  Console UI │    │
│  └─────────────┘    └─────────────┘    │
├─────────────────────────────────────────┤
│              Core Layer                 │
│  ┌─────────────┐    ┌─────────────┐    │
│  │     App     │    │    Menu     │    │
│  └─────────────┘    └─────────────┘    │
├─────────────────────────────────────────┤
│             Media Layer                 │
│  ┌──────────┐ ┌──────────┐ ┌──────────┐ │
│  │MediaFile │ │ Player   │ │ Parser   │ │
│  └──────────┘ └──────────┘ └──────────┘ │
└─────────────────────────────────────────┘
```

### Design Principles
- **Separation of Concerns**: Each component has a single responsibility
- **Interface Abstraction**: UI and media handling are decoupled
- **Extensibility**: Easy to add new UI components or media handlers
- **Error Resilience**: Graceful handling of failures at all levels

## Code Structure

### Directory Organization
```
├── include/                    # Header files
│   ├── core/                   # Core application logic
│   │   ├── app.hpp            # Main application class
│   │   └── menu.hpp           # Menu system interface
│   ├── media/                  # Media handling
│   │   ├── folder_parser.hpp  # Directory scanning
│   │   ├── media_file.hpp     # File representation
│   │   └── player.hpp         # Playback engine
│   └── ui/                     # User interfaces
│       ├── qt_ui.hpp          # Qt GUI implementation
│       ├── console_ui.hpp     # Console interface
│       └── ui_manager.hpp     # UI abstraction layer
├── src/                        # Implementation files
│   ├── core/                   # Core implementations
│   ├── media/                  # Media implementations
│   ├── ui/                     # UI implementations
│   ├── main.cpp               # Qt GUI entry point
│   └── main_console.cpp       # Console entry point
├── tests/                      # Unit tests
├── docs/                       # Documentation
└── assets/                     # Media files (runtime)
```

### File Naming Conventions
- **Headers**: `snake_case.hpp`
- **Sources**: `snake_case.cpp`
- **Classes**: `PascalCase`
- **Functions/Variables**: `camelCase`
- **Constants**: `UPPER_SNAKE_CASE`

## Core Components

### 1. MediaFile Class

**Purpose**: Represents a single media file with metadata and type detection.

```cpp
class MediaFile {
public:
    enum class Type {
        VIDEO,
        AUDIO,
        IMAGE,
        UNKNOWN
    };
    
    MediaFile(const std::string& path);
    
    // Getters
    std::string getPath() const;
    Type getType() const;
    std::string getFormattedSize() const;
    std::string getTypeString() const;
    
private:
    std::string path_;
    Type type_;
    std::size_t size_;
    
    Type detectType() const;
    std::string formatSize(std::size_t bytes) const;
};
```

**Key Features**:
- **Type Detection**: Automatic file type detection based on extension
- **Size Formatting**: Human-readable file size display
- **Metadata Caching**: Efficient metadata storage
- **Path Validation**: Ensures file exists and is readable

**Supported Extensions**:
```cpp
// In detectType() method
static const std::unordered_set<std::string> VIDEO_EXTENSIONS = {
    ".mp4", ".avi", ".mkv", ".mov", ".wmv", ".flv", ".webm", ".m4v"
};
static const std::unordered_set<std::string> AUDIO_EXTENSIONS = {
    ".mp3", ".wav", ".ogg", ".flac", ".aac", ".m4a", ".wma"
};
static const std::unordered_set<std::string> IMAGE_EXTENSIONS = {
    ".jpg", ".jpeg", ".png", ".gif", ".bmp", ".tiff", ".webp"
};
```

### 2. FolderParser Class

**Purpose**: Recursively scans directories for media files and manages the media library.

```cpp
class FolderParser {
public:
    FolderParser(const std::string& folderPath);
    
    std::vector<MediaFile> getMediaFiles();
    bool hasMediaFiles() const;
    std::string getLastError() const;
    
private:
    std::string folderPath_;
    std::vector<MediaFile> mediaFiles_;
    std::string lastError_;
    
    void scanDirectory(const std::string& path);
    bool isMediaFile(const std::string& filename) const;
    void sortMediaFiles();
};
```

**Key Features**:
- **Recursive Scanning**: Traverses subdirectories automatically
- **File Filtering**: Only includes supported media types
- **Error Handling**: Tracks and reports scanning errors
- **Sorting**: Alphabetical organization of discovered files
- **Caching**: Avoids unnecessary rescans

**Scanning Algorithm**:
1. **Directory Traversal**: Uses filesystem API to iterate directories
2. **Extension Filtering**: Checks file extensions against supported lists
3. **File Validation**: Verifies file exists and is readable
4. **MediaFile Creation**: Instantiates MediaFile objects for valid files
5. **Sorting**: Alphabetical sort by filename

### 3. Player Class

**Purpose**: Abstract interface for media playback with callback support.

```cpp
class Player {
public:
    using PlaybackCallback = std::function<void(const std::string&)>;
    using ErrorCallback = std::function<void(const std::string&)>;
    
    void setPlaybackCallback(PlaybackCallback callback);
    void setErrorCallback(ErrorCallback callback);
    
    bool canPlay(const MediaFile& file) const;
    void playMedia(const MediaFile& file);
    
protected:
    PlaybackCallback playbackCallback_;
    ErrorCallback errorCallback_;
    
    void notifyPlaybackStarted(const std::string& message);
    void notifyError(const std::string& error);
    
private:
    bool isFormatSupported(MediaFile::Type type) const;
};
```

**Key Features**:
- **Format Validation**: Checks if media format is supported
- **Callback System**: Notifies UI of playback events and errors
- **Error Reporting**: Detailed error messages for troubleshooting
- **Extensible**: Easy to add new playback engines

### 4. QtUI Class

**Purpose**: Qt-based graphical user interface with full media center functionality.

```cpp
class QtUI : public QMainWindow {
    Q_OBJECT
    
public:
    QtUI(const std::vector<MediaFile>& mediaFiles, QWidget* parent = nullptr);
    
private slots:
    void onMediaSelected();
    void onMediaDoubleClicked();
    void onPlaybackFinished();
    void onMediaError(QMediaPlayer::Error error);
    
private:
    // UI Components
    QListWidget* mediaListWidget_;
    QVideoWidget* videoWidget_;
    QLabel* imageLabel_;
    QMediaPlayer* mediaPlayer_;
    QLabel* statusLabel_;
    
    // Data
    std::vector<MediaFile> mediaFiles_;
    Player player_;
    
    // Methods
    void setupUI();
    void setupMediaPlayer();
    void populateMediaList();
    void playSelectedMedia();
    void displayImage(const std::string& imagePath);
    void showError(const std::string& message);
    void updateStatusBar(const std::string& message);
};
```

**UI Components**:
- **Media List**: `QListWidget` showing all discovered media files
- **Video Player**: `QVideoWidget` for video playback
- **Image Viewer**: `QLabel` for image display
- **Status Bar**: Information and error display
- **Keyboard Handler**: Custom key event processing

**Key Features**:
- **Dual Display Mode**: Separate handling for videos and images
- **Keyboard Navigation**: Arrow keys, Enter, Space, Escape
- **Mouse Support**: Click and double-click interactions
- **Error Dialogs**: User-friendly error reporting
- **Status Updates**: Real-time playback status

### 5. ConsoleUI Class

**Purpose**: Text-based interface for environments without GUI support.

```cpp
class ConsoleUI {
public:
    ConsoleUI(const std::vector<MediaFile>& mediaFiles);
    void run();
    
private:
    std::vector<MediaFile> mediaFiles_;
    Player player_;
    bool running_;
    
    void displayMediaList();
    void displayHelp();
    void handleUserInput();
    void playMediaByIndex(int index);
    void clearScreen();
    std::string getUserInput();
};
```

**Key Features**:
- **System Integration**: Launches files with system default applications
- **Command Interface**: Simple text-based commands
- **Error Handling**: Clear error messages in console
- **Cross-platform**: Works on any system with terminal access

## Build System

### CMake Configuration

The build system supports dual targets and flexible Qt version detection:

```cmake
cmake_minimum_required(VERSION 3.16)
project(epikodi VERSION 1.0.0 LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt5 (fallback to Qt6)
find_package(Qt5 QUIET COMPONENTS Core Widgets Multimedia MultimediaWidgets)
if(NOT Qt5_FOUND)
    find_package(Qt6 REQUIRED COMPONENTS Core Widgets Multimedia MultimediaWidgets)
    set(QT_VERSION_MAJOR 6)
else()
    set(QT_VERSION_MAJOR 5)
endif()
```

### Build Targets

#### 1. GUI Target (epikodi)
```cmake
# Core source files
set(CORE_SOURCES
    src/core/app.cpp
    src/core/menu.cpp
)

set(MEDIA_SOURCES
    src/media/media_file.cpp
    src/media/folder_parser.cpp
    src/media/player.cpp
)

set(UI_SOURCES
    src/ui/qt_ui.cpp
)

# GUI executable
add_executable(epikodi
    src/main.cpp
    ${CORE_SOURCES}
    ${MEDIA_SOURCES}
    ${UI_SOURCES}
)

# Link Qt libraries
target_link_libraries(epikodi
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Multimedia
    Qt${QT_VERSION_MAJOR}::MultimediaWidgets
)
```

#### 2. Console Target (epikodi_console)
```cmake
# Console executable
add_executable(epikodi_console
    src/main_console.cpp
    ${CORE_SOURCES}
    ${MEDIA_SOURCES}
    src/ui/console_ui.cpp
)

# Link filesystem library
target_link_libraries(epikodi_console stdc++fs)
```

### Build Process

#### Debug Build
```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j$(nproc)
```

#### Release Build
```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
```

### Custom CMake Tasks

The build system includes custom tasks for common operations:

```cmake
# Clean and rebuild
add_custom_target(rebuild
    COMMAND ${CMAKE_BUILD_TOOL} clean
    COMMAND ${CMAKE_BUILD_TOOL}
    COMMENT "Clean and rebuild all targets"
)

# Run GUI application
add_custom_target(run
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/bin/epikodi
    DEPENDS epikodi
    COMMENT "Run the GUI application"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
```

## Development Guidelines

### Code Style

#### C++ Standards
- **Language Version**: C++17 minimum
- **Memory Management**: Smart pointers preferred over raw pointers
- **Error Handling**: Exceptions for exceptional cases, return codes for normal failures
- **Const Correctness**: Mark methods const when they don't modify state

#### Naming Conventions
```cpp
// Classes: PascalCase
class MediaFile {
    // Public methods: camelCase
    std::string getPath() const;
    
    // Private members: trailing underscore
private:
    std::string path_;
    MediaFile::Type type_;
};

// Constants: UPPER_SNAKE_CASE
static const int MAX_FILE_SIZE = 1024 * 1024 * 1024; // 1GB

// Namespaces: snake_case
namespace media_utils {
    // Functions: camelCase
    bool isValidExtension(const std::string& ext);
}
```

#### Documentation Standards
```cpp
/**
 * @brief Represents a media file with metadata
 * 
 * The MediaFile class encapsulates file information including path,
 * type detection, and size formatting. It automatically detects the
 * media type based on file extension and provides human-readable
 * size formatting.
 * 
 * @see FolderParser for media file discovery
 * @see Player for playback capabilities
 */
class MediaFile {
public:
    /**
     * @brief Constructs a MediaFile from a file path
     * @param path Absolute or relative path to the media file
     * @throws std::runtime_error if file doesn't exist
     */
    explicit MediaFile(const std::string& path);
```

### Error Handling Strategy

#### 1. Error Categories
```cpp
namespace errors {
    enum class Category {
        FILE_NOT_FOUND,
        UNSUPPORTED_FORMAT,
        PLAYBACK_FAILURE,
        UI_ERROR,
        SYSTEM_ERROR
    };
    
    class MediaException : public std::exception {
    public:
        MediaException(Category category, const std::string& message)
            : category_(category), message_(message) {}
            
        const char* what() const noexcept override {
            return message_.c_str();
        }
        
        Category getCategory() const { return category_; }
        
    private:
        Category category_;
        std::string message_;
    };
}
```

#### 2. Error Handling Patterns
```cpp
// Function that can fail - return optional or expected
std::optional<MediaFile> createMediaFile(const std::string& path) {
    try {
        return MediaFile(path);
    } catch (const std::exception& e) {
        // Log error
        std::cerr << "Failed to create MediaFile: " << e.what() << std::endl;
        return std::nullopt;
    }
}

// UI error handling - user-friendly messages
void QtUI::handlePlaybackError(const std::string& error) {
    QString userMessage = "Unable to play media file.\n";
    
    if (error.find("codec") != std::string::npos) {
        userMessage += "This might be due to missing codecs.";
    } else if (error.find("permission") != std::string::npos) {
        userMessage += "Check file permissions.";
    } else {
        userMessage += "Please try a different file.";
    }
    
    QMessageBox::warning(this, "Playback Error", userMessage);
}
```

### Performance Considerations

#### 1. File System Operations
```cpp
// Efficient directory scanning
void FolderParser::scanDirectory(const std::string& path) {
    // Use filesystem::directory_iterator for better performance
    for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) {
        if (entry.is_regular_file()) {
            const auto& filePath = entry.path();
            
            // Quick extension check before creating MediaFile
            if (isMediaFile(filePath.filename().string())) {
                try {
                    mediaFiles_.emplace_back(filePath.string());
                } catch (const std::exception& e) {
                    // Log and continue
                    lastError_ = e.what();
                }
            }
        }
    }
}
```

#### 2. Memory Management
```cpp
// Use smart pointers for Qt objects with parent-child relationships
class QtUI : public QMainWindow {
private:
    // Qt handles cleanup through parent-child relationships
    QListWidget* mediaListWidget_;  // Raw pointer, owned by QMainWindow
    
    // Use smart pointers for objects without clear ownership
    std::unique_ptr<Player> player_;
    std::shared_ptr<MediaFile> currentMedia_;
};
```

#### 3. UI Responsiveness
```cpp
// Long-running operations should be async
void QtUI::scanMediaAsync() {
    // Use QtConcurrent for background operations
    auto future = QtConcurrent::run([this]() {
        FolderParser parser("assets");
        return parser.getMediaFiles();
    });
    
    // Watch for completion
    auto watcher = new QFutureWatcher<std::vector<MediaFile>>(this);
    connect(watcher, &QFutureWatcher<std::vector<MediaFile>>::finished,
            this, &QtUI::onMediaScanComplete);
    watcher->setFuture(future);
}
```

## Testing

### Unit Testing Framework

EpiKodi uses a simple custom testing framework:

```cpp
// tests/test_media_file.cpp
#include "../include/media/media_file.hpp"
#include <cassert>
#include <iostream>

void testMediaFileCreation() {
    // Test valid media file
    MediaFile video("test.mp4");
    assert(video.getType() == MediaFile::Type::VIDEO);
    assert(video.getTypeString() == "Video");
    
    std::cout << "✓ MediaFile creation test passed" << std::endl;
}

void testTypeDetection() {
    // Test various file types
    assert(MediaFile("test.mp4").getType() == MediaFile::Type::VIDEO);
    assert(MediaFile("test.mp3").getType() == MediaFile::Type::AUDIO);
    assert(MediaFile("test.png").getType() == MediaFile::Type::IMAGE);
    assert(MediaFile("test.txt").getType() == MediaFile::Type::UNKNOWN);
    
    std::cout << "✓ Type detection test passed" << std::endl;
}

int main() {
    try {
        testMediaFileCreation();
        testTypeDetection();
        
        std::cout << "All MediaFile tests passed!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }
}
```

### Test Execution
```bash
# Build tests
cd build
make

# Run specific test
./tests/test_media_file

# Run all tests (if test runner exists)
make test
```

### Integration Testing

#### GUI Testing Strategy
1. **Manual Testing**: User interaction scenarios
2. **Automated Testing**: Qt Test framework for widget testing
3. **Visual Testing**: Screenshot comparison for UI regression

#### Console Testing
1. **Input/Output Testing**: Scripted interaction testing
2. **Command Testing**: Verify all console commands work correctly
3. **Error Scenario Testing**: Test error handling paths

## Platform Support

### Linux/Ubuntu
- **Primary Platform**: Fully supported with native Qt5
- **Package Dependencies**: Managed through APT
- **Audio/Video**: GStreamer backend
- **File System**: Native filesystem API

### WSL (Windows Subsystem for Linux)
- **Display**: Requires X11 forwarding or Wayland
- **Audio**: Limited support, may have quality issues
- **Performance**: Slightly reduced due to virtualization
- **Codecs**: Same as Linux but may need additional configuration

### Future Platforms

#### Windows Native
**Planned Features**:
- DirectShow backend for media playback
- Windows Explorer integration
- Native Windows installer

**Implementation Considerations**:
```cpp
#ifdef _WIN32
    // Windows-specific implementations
    #include <windows.h>
    
    class WindowsPlayer : public Player {
        // DirectShow implementation
    };
#endif
```

#### macOS
**Planned Features**:
- AVFoundation backend
- Finder integration
- macOS app bundle

### Cross-Platform Abstractions

#### File System Abstraction
```cpp
namespace filesystem_utils {
    // Cross-platform path handling
    std::string normalizePath(const std::string& path);
    bool fileExists(const std::string& path);
    std::vector<std::string> listDirectory(const std::string& path);
    
    // Platform-specific implementations
    #ifdef __linux__
        // Linux implementation
    #elif _WIN32
        // Windows implementation
    #elif __APPLE__
        // macOS implementation
    #endif
}
```

---

## Contributing

### Development Setup
1. **Clone Repository**: `git clone https://github.com/iiishuu/EpiKodi.git`
2. **Install Dependencies**: Follow platform-specific setup in README
3. **Build Project**: Use CMake build process
4. **Run Tests**: Execute test suite before committing

### Contribution Guidelines
- Follow the established code style and naming conventions
- Write comprehensive tests for new features
- Update documentation for API changes
- Ensure cross-platform compatibility where applicable

### Code Review Process
- All changes must pass automated tests
- Code style must be consistent with existing codebase
- Performance impact should be minimal
- Documentation must be updated for user-facing changes

---

*For user-focused documentation, see [USER_GUIDE.md](USER_GUIDE.md)*