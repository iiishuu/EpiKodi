/*
** EPITECH PROJECT, 2025
** EpiKodi [WSL: Ubuntu]
** File description:
** app - Core application logic
*/

#include "core/app.hpp"
#include "media/folder_parser.hpp"
#include "media/media_file.hpp"
#include "media/player.hpp"

#include <iostream>
#include <vector>

namespace epikodi {
    void App::run(const std::string &mediaPath)
    {
        std::cout << "[App] Scanning media in: " << mediaPath << std::endl;
        auto files = FolderParser::listFiles(mediaPath);
        
        if (files.empty()) {
            std::cout << "[App] No media found." << std::endl;
            return;
        }
        
        std::cout << "[App] Found " << files.size() << " media file(s):" << std::endl;
        for (size_t i = 0; i < files.size(); i++) {
            std::cout << " " << (i + 1) << ". " << files[i].getInfo() << std::endl;
        }
        
        // Demo playback simulation
        Player player;
        std::cout << "[App] Simulating playback of first file..." << std::endl;
        player.play(files[0].getName());
        player.pause();
        player.stop();
    }
}
