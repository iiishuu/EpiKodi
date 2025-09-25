/*
** EPITECH PROJECT, 2025
** EpiKodi [WSL: Ubuntu]
** File description:
** media_file
*/

#include "media/media_file.hpp"
#include <filesystem>
#include <algorithm>
#include <sstream>
#include <iomanip>

namespace epikodi {
    
    MediaFile::MediaFile(const std::string &n, const std::string &t, size_t s)
        : name(n), type(t), size(s), path(""), mediaType(MediaType::UNKNOWN) {
        // Extrait l'extension du nom
        size_t dotPos = name.find_last_of('.');
        if (dotPos != std::string::npos) {
            extension = name.substr(dotPos);
            std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
            mediaType = determineMediaType(extension);
        }
    }
    
    MediaFile::MediaFile(const std::string &path) : path(path) {
        analyzeFile(path);
    }

    std::string MediaFile::getName() const { return name; }
    std::string MediaFile::getType() const { return type; }
    std::string MediaFile::getPath() const { return path; }
    size_t MediaFile::getSize() const { return size; }
    
    MediaType MediaFile::getMediaType() const {
        return mediaType;
    }
    
    std::string MediaFile::getSizeFormatted() const {
        return formatFileSize(size);
    }
    
    std::string MediaFile::getExtension() const {
        return extension;
    }
    
    std::string MediaFile::getInfo() const {
        std::ostringstream info;
        info << name;
        
        // Type de média avec icône
        switch (mediaType) {
            case MediaType::VIDEO: info << " [🎥 Vidéo]"; break;
            case MediaType::AUDIO: info << " [🎵 Audio]"; break;
            case MediaType::IMAGE: info << " [🖼️ Image]"; break;
            case MediaType::UNKNOWN: info << " [❓ Inconnu]"; break;
        }
        
        info << " - " << getSizeFormatted();
        return info.str();
    }

    // Méthodes statiques
    MediaType MediaFile::determineMediaType(const std::string &extension) {
        std::string ext = extension;
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        
        // Extensions vidéo
        if (ext == ".mp4" || ext == ".avi" || ext == ".mkv" || ext == ".mov" || 
            ext == ".wmv" || ext == ".flv" || ext == ".webm" || ext == ".m4v") {
            return MediaType::VIDEO;
        }
        
        // Extensions audio
        if (ext == ".mp3" || ext == ".wav" || ext == ".ogg" || ext == ".flac" ||
            ext == ".aac" || ext == ".m4a" || ext == ".wma") {
            return MediaType::AUDIO;
        }
        
        // Extensions image
        if (ext == ".jpg" || ext == ".jpeg" || ext == ".png" || ext == ".gif" ||
            ext == ".bmp" || ext == ".tiff" || ext == ".webp") {
            return MediaType::IMAGE;
        }
        
        return MediaType::UNKNOWN;
    }

    std::string MediaFile::formatFileSize(size_t bytes) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1);
        
        if (bytes >= 1024 * 1024 * 1024) {
            oss << (bytes / (1024.0 * 1024.0 * 1024.0)) << " GB";
        } else if (bytes >= 1024 * 1024) {
            oss << (bytes / (1024.0 * 1024.0)) << " MB";
        } else if (bytes >= 1024) {
            oss << (bytes / 1024.0) << " KB";
        } else {
            oss << bytes << " B";
        }
        
        return oss.str();
    }
    
    void MediaFile::analyzeFile(const std::string &fullPath) {
        path = fullPath;
        
        // Extrait le nom du fichier
        size_t slashPos = fullPath.find_last_of('/');
        if (slashPos != std::string::npos) {
            name = fullPath.substr(slashPos + 1);
        } else {
            name = fullPath;
        }
        
        // Extrait l'extension
        size_t dotPos = name.find_last_of('.');
        if (dotPos != std::string::npos) {
            extension = name.substr(dotPos);
            std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
        }
        
        // Détermine le type
        mediaType = determineMediaType(extension);
        
        // Type en string pour compatibilité
        switch (mediaType) {
            case MediaType::VIDEO: type = "video"; break;
            case MediaType::AUDIO: type = "audio"; break;
            case MediaType::IMAGE: type = "image"; break;
            case MediaType::UNKNOWN: type = "unknown"; break;
        }
        
        // Taille du fichier
        try {
            if (std::filesystem::exists(fullPath)) {
                size = std::filesystem::file_size(fullPath);
            } else {
                size = 0;
            }
        } catch (...) {
            size = 0;
        }
    }
}
