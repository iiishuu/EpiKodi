/*
** EPITECH PROJECT, 2025
** EpiKodi [WSL: Ubuntu]
** File description:
** media_file
*/

#ifndef MEDIA_FILE_HPP
#define MEDIA_FILE_HPP

#include <string>

namespace epikodi {
    enum class MediaType {
        VIDEO,
        AUDIO,
        IMAGE,
        UNKNOWN
    };
    
    class MediaFile {
    public:
        MediaFile(const std::string &name, const std::string &type, size_t size);
        MediaFile(const std::string &path); // Nouveau constructeur avec analyse automatique

        std::string getName() const;
        std::string getType() const;
        std::string getPath() const;
        size_t getSize() const;
        MediaType getMediaType() const;
        std::string getSizeFormatted() const;
        std::string getExtension() const;
        std::string getInfo() const; // Info complète formatée
        
        // Méthodes statiques pour l'analyse
        static MediaType determineMediaType(const std::string &extension);
        static std::string formatFileSize(size_t bytes);

    private:
        std::string name;
        std::string type;
        std::string path;
        size_t size;
        MediaType mediaType;
        std::string extension;
        
        void analyzeFile(const std::string &fullPath);
    };
}

#endif // MEDIA_FILE_HPP
