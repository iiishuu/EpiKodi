/*
** EPITECH PROJECT, 2025
** EpiKodi [WSL: Ubuntu]
** File description:
** player
*/

#include "media/player.hpp"
#include <iostream>
#include <algorithm>
#include <vector>

namespace epikodi {
    Player::Player() {
        statusCallback = [](const std::string& msg) {
            std::cout << "[Player] " << msg << std::endl;
        };
        errorCallback = [](const std::string& err) {
            std::cerr << "[Player ERROR] " << err << std::endl;
        };
    }

    Player::~Player() {}

    void Player::play(const std::string &file) {
        if (statusCallback) statusCallback("Playing: " + file);
        
        if (!canPlayFile(file)) {
            if (errorCallback) errorCallback("Unsupported format: " + file);
            return;
        }
    }

    void Player::pause() {
        if (statusCallback) statusCallback("Paused");
    }

    void Player::stop() {
        if (statusCallback) statusCallback("Stopped");
    }
    
    void Player::setErrorCallback(std::function<void(const std::string&)> callback) {
        errorCallback = callback;
    }
    
    void Player::setStatusCallback(std::function<void(const std::string&)> callback) {
        statusCallback = callback;
    }
    
    bool Player::canPlayFile(const std::string &file) {
        // Extensions supportées couramment sous Linux
        std::vector<std::string> supported = {".mp4", ".avi", ".mkv", ".mov", ".wmv", ".mp3", ".wav", ".ogg"};
        
        std::string lowercase = file;
        std::transform(lowercase.begin(), lowercase.end(), lowercase.begin(), ::tolower);
        
        for (const auto& ext : supported) {
            if (lowercase.find(ext) != std::string::npos) {
                return true;
            }
        }
        return false;
    }
    
    std::string Player::getSupportedFormats() {
        return "Supported: MP4, AVI, MKV, MOV, WMV, MP3, WAV, OGG";
    }
}
