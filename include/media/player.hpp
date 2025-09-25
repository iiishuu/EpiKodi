/*
** EPITECH PROJECT, 2025
** EpiKodi [WSL: Ubuntu]
** File description:
** player
*/

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <functional>

namespace epikodi {
    class Player {
    private:
        std::function<void(const std::string&)> errorCallback;
        std::function<void(const std::string&)> statusCallback;
        
    public:
        Player();
        ~Player();
        
        void play(const std::string &file);
        void pause();
        void stop();
        
        // Callbacks pour les erreurs et status
        void setErrorCallback(std::function<void(const std::string&)> callback);
        void setStatusCallback(std::function<void(const std::string&)> callback);
        
        // Test de codec/format
        bool canPlayFile(const std::string &file);
        std::string getSupportedFormats();
    };
}

#endif // PLAYER_HPP
