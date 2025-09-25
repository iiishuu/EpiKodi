/*
** EPITECH PROJECT, 2025
** EpiKodi [WSL: Ubuntu]
** File description:
** folder_parser
*/

#include "media/folder_parser.hpp"
#include <filesystem>
#include <iostream>
#include <algorithm>

namespace fs = std::filesystem;

namespace epikodi {
    std::vector<MediaFile> FolderParser::listFiles(const std::string &path)
    {
        std::vector<MediaFile> files;
        
        std::cout << "[FolderParser] Scanning directory: " << path << std::endl;

        try {
            if (!fs::exists(path)) {
                std::cerr << "[FolderParser] Error: Directory '" << path << "' does not exist" << std::endl;
                return files;
            }
            
            if (!fs::is_directory(path)) {
                std::cerr << "[FolderParser] Error: '" << path << "' is not a directory" << std::endl;
                return files;
            }
            
            int mediaCount = 0;
            for (const auto &entry : fs::directory_iterator(path)) {
                if (entry.is_regular_file()) {
                    std::string filePath = entry.path().string();
                    MediaFile mediaFile(filePath);
                    
                    // Ne garde que les fichiers média reconnus
                    if (mediaFile.getMediaType() != MediaType::UNKNOWN) {
                        files.push_back(mediaFile);
                        mediaCount++;
                        std::cout << "[FolderParser] Found: " << mediaFile.getInfo() << std::endl;
                    }
                }
            }
            
            // Tri par nom
            std::sort(files.begin(), files.end(), [](const MediaFile& a, const MediaFile& b) {
                return a.getName() < b.getName();
            });
            
            std::cout << "[FolderParser] Found " << mediaCount << " media file(s)" << std::endl;
            
        } catch (const std::exception &e) {
            std::cerr << "[FolderParser] Error: " << e.what() << std::endl;
        }

        return files;
    }
    
    std::vector<MediaFile> FolderParser::listFilesByType(const std::string &path, MediaType type)
    {
        auto allFiles = listFiles(path);
        std::vector<MediaFile> filteredFiles;
        
        for (const auto& file : allFiles) {
            if (file.getMediaType() == type) {
                filteredFiles.push_back(file);
            }
        }
        
        return filteredFiles;
    }
    
    bool FolderParser::hasMediaFiles(const std::string &path)
    {
        try {
            for (const auto &entry : fs::directory_iterator(path)) {
                if (entry.is_regular_file()) {
                    MediaFile mediaFile(entry.path().string());
                    if (mediaFile.getMediaType() != MediaType::UNKNOWN) {
                        return true;
                    }
                }
            }
        } catch (const std::exception &e) {
            std::cerr << "[FolderParser] Error checking media files: " << e.what() << std::endl;
        }
        return false;
    }
}
