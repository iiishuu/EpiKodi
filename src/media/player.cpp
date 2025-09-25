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
#include <fstream>
#include <filesystem>

namespace epikodi {
    Player::Player() : currentState(State::STOPPED) {
        statusCallback = [](const std::string& msg) {
            std::cout << "[Player] " << msg << std::endl;
        };
        errorCallback = [](const std::string& err) {
            std::cerr << "[Player ERROR] " << err << std::endl;
        };
    }

    Player::~Player() {}

    void Player::play(const std::string &file) {
        // Check if file exists
        if (!std::filesystem::exists(file)) {
            currentState = State::ERROR;
            if (errorCallback) errorCallback("File not found: " + file);
            return;
        }

        // Check if file is corrupted
        if (isFileCorrupted(file)) {
            currentState = State::ERROR;
            if (errorCallback) errorCallback("File corrupted: " + file);
            return;
        }
        
        // Check format support
        if (!canPlayFile(file)) {
            currentState = State::ERROR;
            if (errorCallback) errorCallback("Unsupported format: " + file);
            return;
        }
        
        currentState = State::PLAYING;
        if (statusCallback) statusCallback("Playing: " + file);
        
        // Specific messages for different formats
        std::string ext = file.substr(file.find_last_of('.'));
        if (ext == ".mp3" || ext == ".wav") {
            if (statusCallback) statusCallback("Audio playback started");
        } else if (ext == ".mp4") {
            if (statusCallback) statusCallback("Video playback started");
        }
    }

    void Player::pause() {
        if (currentState == State::PLAYING) {
            currentState = State::PAUSED;
            if (statusCallback) statusCallback("Paused");
        } else if (currentState == State::PAUSED) {
            currentState = State::PLAYING;
            if (statusCallback) statusCallback("Resumed");
        }
    }

    void Player::stop() {
        currentState = State::STOPPED;
        if (statusCallback) statusCallback("Stopped");
    }
    
    void Player::next() {
        if (statusCallback) statusCallback("Next track");
    }
    
    Player::State Player::getState() const {
        return currentState;
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
    
    bool Player::isFileCorrupted(const std::string &file) {
        std::ifstream fileStream(file, std::ios::binary);
        if (!fileStream.is_open()) {
            return true; // Can't open = corrupted
        }
        
        // Basic corruption check - file should have some content
        fileStream.seekg(0, std::ios::end);
        std::streamsize fileSize = fileStream.tellg();
        
        if (fileSize < 50) { // Files smaller than 50 bytes suspicious
            return true;
        }
        
        return false; // File seems OK
    }
}
