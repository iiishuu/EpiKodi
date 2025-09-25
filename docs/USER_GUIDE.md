# EpiKodi User Guide

Welcome to EpiKodi, your personal media center application! This guide will help you get started and make the most of all available features.

## Table of Contents
1. [Getting Started](#getting-started)
2. [Interface Overview](#interface-overview)
3. [Media Management](#media-management)
4. [Navigation & Controls](#navigation--controls)
5. [Playback Features](#playback-features)
6. [Troubleshooting](#troubleshooting)

## Getting Started

### First Launch
1. **Add your media files**: Place your videos, music, and images in the `assets/` folder
2. **Launch EpiKodi**: 
   - For GUI: Run `./build/bin/epikodi`
   - For console: Run `./build/bin/epikodi_console`
3. **Start exploring**: Your media files will be automatically detected and displayed

### System Requirements
- **Operating System**: Linux (Ubuntu/Debian recommended), WSL support
- **Hardware**: Minimum 2GB RAM, graphics card with OpenGL support
- **Media Codecs**: Automatically installed with the setup script

## Interface Overview

### Qt GUI Interface

#### Main Window Layout
```
┌─────────────────────────────────────────────────────────┐
│ EpiKodi Media Center                                    │
├─────────────────────┬───────────────────────────────────┤
│                     │                                   │
│   Media Library     │        Playback Area             │
│                     │                                   │
│  📁 Folder1/        │   [Video Player / Image Viewer]  │
│  🎥 video.mp4       │                                   │
│  🎵 music.mp3       │                                   │
│  🖼️ image.png       │                                   │
│                     │                                   │
│  File Info:         │                                   │
│  Type: Video        │                                   │
│  Size: 150 MB       │                                   │
│                     │                                   │
└─────────────────────┴───────────────────────────────────┘
```

#### Interface Elements
- **Media Library (Left Panel)**: Displays all discovered media files with type icons
- **Playback Area (Right Panel)**: Shows videos or images, provides playback controls
- **Status Bar**: Displays current file information and system status
- **File Info Section**: Shows detailed metadata for selected files

### Console Interface

#### Console Layout
```
========================================
           EpiKodi Console
========================================

Media Library:
1. 🎥 video.mp4 (150.2 MB)
2. 🎵 song.mp3 (5.8 MB)  
3. 🖼️ photo.png (2.1 MB)
4. 🎥 movie.avi (1.2 GB)

Commands:
- Enter number (1-4) to play/view media
- 'l' or 'list' to refresh library
- 'h' or 'help' for help
- 'q' or 'quit' to exit

> _
```

## Media Management

### Supported File Types

#### Video Files 🎥
- **Common formats**: MP4, AVI, MKV, MOV, WMV
- **Additional formats**: FLV, WebM, M4V
- **Features**: Full playback control, progress tracking

#### Audio Files 🎵
- **Lossless**: FLAC, WAV
- **Compressed**: MP3, OGG, AAC, M4A, WMA
- **Features**: Background playback, metadata display

#### Image Files 🖼️
- **Standard formats**: JPG, PNG, GIF, BMP
- **Advanced formats**: TIFF, WebP
- **Features**: Full-screen viewing, zoom capabilities

### File Organization

#### Recommended Directory Structure
```
assets/
├── Movies/
│   ├── Action/
│   │   └── movie1.mp4
│   └── Comedy/
│       └── movie2.avi
├── Music/
│   ├── Rock/
│   │   └── song1.mp3
│   └── Jazz/
│       └── song2.flac
└── Photos/
    ├── Vacation/
    │   └── photo1.jpg
    └── Family/
        └── photo2.png
```

#### File Detection
- **Automatic scanning**: EpiKodi recursively scans the `assets/` folder
- **Real-time updates**: New files are detected when you restart the application
- **Format validation**: Only supported media types are displayed

## Navigation & Controls

### Qt GUI Controls

#### Keyboard Navigation
| Key | Action |
|-----|--------|
| `↑` `↓` | Navigate media library |
| `Enter` | Play/view selected media |
| `Space` | Play/pause (during playback) |
| `Escape` | Stop playback/return to library |
| `F` | Toggle fullscreen (video only) |

#### Mouse Navigation
- **Single click**: Select media file
- **Double-click**: Play/view media file
- **Right-click**: Context menu (future feature)

### Console Controls

#### Available Commands
| Command | Shortcut | Description |
|---------|----------|-------------|
| `1-9` | Number keys | Play/view media by index |
| `list` | `l` | Refresh and display media library |
| `help` | `h` | Show available commands |
| `quit` | `q` | Exit application |
| `exit` | - | Alternative exit command |

## Playback Features

### Video Playback
- **Full-screen mode**: Press `F` to toggle fullscreen
- **Playback controls**: Play, pause, stop, seek
- **Audio synchronization**: Automatic audio/video sync
- **Format support**: Hardware-accelerated when available

### Audio Playback
- **Background playback**: Continue using the interface while music plays
- **Metadata display**: Artist, title, album information (when available)
- **Volume control**: System volume integration

### Image Viewing
- **Direct display**: Images show immediately without "playing"
- **Full resolution**: Native resolution display with smooth scaling
- **Format support**: All common image formats

## Advanced Features

### Media Information Display
- **File details**: Name, type, size, format
- **Metadata extraction**: Duration, resolution, codec information
- **File system info**: Creation date, modification date

### Error Handling
- **Graceful failures**: Clear error messages for unsupported files
- **Recovery options**: Suggestions for fixing common issues
- **Fallback modes**: Console mode when GUI fails

### System Integration
- **Console fallback**: System player integration for unsupported formats
- **Platform compatibility**: Native behavior on different systems
- **Codec detection**: Automatic codec requirement detection

## Troubleshooting

### Common Issues

#### "No media files found"
**Cause**: Empty assets folder or unsupported file types
**Solution**: 
1. Verify files are in the `assets/` folder
2. Check file formats are supported
3. Restart the application

#### Video plays but no audio
**Cause**: Missing audio codecs or WSL audio issues
**Solution**:
1. Install audio codecs: `sudo apt install ubuntu-restricted-extras`
2. Check WSL audio configuration
3. Test with different video files

#### Application won't start (GUI)
**Cause**: Missing Qt libraries or display issues
**Solution**:
1. Install Qt dependencies: `sudo apt install qtmultimedia5-dev`
2. Set display variable: `export DISPLAY=:0` (WSL)
3. Use console version: `./bin/epikodi_console`

#### Images don't display properly
**Cause**: Corrupted image files or unsupported format
**Solution**:
1. Verify image file integrity
2. Convert to supported format (PNG/JPG)
3. Check file permissions

### Performance Tips

#### For Better Video Playback
- Use hardware-accelerated formats (H.264/H.265)
- Ensure sufficient disk space for temporary files
- Close other applications during playback

#### For Large Media Libraries
- Organize files in subdirectories
- Use meaningful file names
- Remove duplicate or corrupted files

### Getting Help

#### Log Files
- **Location**: `~/.epikodi/logs/` (future feature)
- **Content**: Error messages and debug information

#### Support Resources
- Check the [Technical Documentation](TECHNICAL_GUIDE.md) for advanced topics
- Review error messages for specific solutions
- Test with sample media files to isolate issues

---

**Enjoy your media with EpiKodi!** 🎬🎵🖼️