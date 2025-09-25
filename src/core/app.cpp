/*
** EPITECH PROJECT, 2025
** EpiKodi [WSL: Ubuntu]
** File description:
** app - Core application logic and main entry point
*/

#include "core/app.hpp"
#include "media/folder_parser.hpp"
#include "media/media_file.hpp"
#include "media/player.hpp"

#include <iostream>
#include <vector>

namespace epikodi {
    /**
     * @brief Main application runner - scans media directory and demonstrates playback
     * @param mediaPath Path to the directory containing media files
     * 
     * This function serves as the core application logic:
     * 1. Scans the specified directory for media files
     * 2. Displays found media with formatted information
     * 3. Demonstrates basic playback functionality
     */
    void App::run(const std::string &mediaPath)
    {
        std::cout << "[App] Scanning media in: " << mediaPath << std::endl;
        
        // Use FolderParser to discover all media files recursively
        auto files = FolderParser::listFiles(mediaPath);
        
        // Handle case where no media files are found
        if (files.empty()) {
            std::cout << "[App] No media found." << std::endl;
            return;
        }
        
        // Display all discovered media files with metadata
        std::cout << "[App] Found " << files.size() << " media file(s):" << std::endl;
        for (size_t i = 0; i < files.size(); i++) {
            std::cout << " " << (i + 1) << ". " << files[i].getInfo() << std::endl;
        }
        
        // Demonstrate basic playback functionality with the first file
        Player player;
        std::cout << "[App] Simulating playback of first file..." << std::endl;
        player.play(files[0].getName());
        player.pause();
        player.stop();
    }
}
