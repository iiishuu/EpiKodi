/*
** EPITECH PROJECT, 2025
** EpiKodi [WSL: Ubuntu]
** File description:
** console_ui
*/

#include "ui/console_ui.hpp"
#include "media/media_file.hpp"
#include <iostream>
#include <string>
#include <cstdlib>
#include <limits>

namespace epikodi {
    
    ConsoleUI::ConsoleUI() {}
    ConsoleUI::~ConsoleUI() {}

    void ConsoleUI::openWindow(const std::string &title) {
        printHeader(title);
    }

    void ConsoleUI::closeWindow() {
        std::cout << "\n👋 Merci d'avoir utilisé EpiKodi !\n" << std::endl;
    }

    void ConsoleUI::showMenu(const std::vector<std::string> &mediaFiles) {
        printMediaList(mediaFiles);
        
        if (!mediaFiles.empty()) {
            handleUserInput(mediaFiles);
        }
    }

    void ConsoleUI::printHeader(const std::string &title) {
        std::cout << "\n╔════════════════════════════════════════════════════════════╗\n";
        std::cout << "║                    " << title << "                     ║\n";
        std::cout << "╚════════════════════════════════════════════════════════════╝\n";
        std::cout << "🎬 Interface Console - Navigation simplifiée\n" << std::endl;
    }

    void ConsoleUI::printMediaList(const std::vector<std::string> &mediaFiles) {
        std::cout << "📁 Bibliothèque Multimédia:\n";
        std::cout << "─────────────────────────────────────────────────────────────\n";
        
        if (mediaFiles.empty()) {
            std::cout << "❌ Aucun fichier média trouvé dans le dossier 'assets/'\n";
            std::cout << "💡 Placez vos fichiers vidéo, audio ou image dans ce dossier.\n";
            std::cout << "\nFormats supportés:\n";
            std::cout << "  🎥 Vidéo: .mp4, .avi, .mkv, .mov, .wmv, .flv, .webm, .m4v\n";
            std::cout << "  🎵 Audio: .mp3, .wav, .ogg, .flac, .aac, .m4a, .wma\n";
            std::cout << "  🖼️  Image: .jpg, .jpeg, .png, .gif, .bmp, .tiff, .webp\n";
            return;
        }

        for (size_t i = 0; i < mediaFiles.size(); ++i) {
            std::cout << "[" << (i + 1) << "] " << mediaFiles[i] << std::endl;
        }
        
        std::cout << "\n📊 Total: " << mediaFiles.size() << " fichier(s) média\n";
    }

    void ConsoleUI::handleUserInput(const std::vector<std::string> &mediaFiles) {
        showHelp();
        
        while (true) {
            std::cout << "\n🎯 Votre choix: ";
            std::string input;
            std::getline(std::cin, input);
            
            if (input.empty()) continue;
            
            if (input == "q" || input == "quit" || input == "exit") {
                break;
            } else if (input == "h" || input == "help") {
                showHelp();
                continue;
            } else if (input == "l" || input == "list") {
                printMediaList(mediaFiles);
                continue;
            }
            
            try {
                size_t choice = std::stoul(input);
                if (choice >= 1 && choice <= mediaFiles.size()) {
                    // Extrait le nom du fichier de la string avec info
                    std::string fileInfo = mediaFiles[choice - 1];
                    size_t spacePos = fileInfo.find(' ');
                    std::string fileName = (spacePos != std::string::npos) ? 
                                         fileInfo.substr(0, spacePos) : fileInfo;
                    
                    std::cout << "🎬 Lancement de: " << fileName << std::endl;
                    launchExternalPlayer(fileName);
                } else {
                    std::cout << "❌ Numéro invalide. Tapez un nombre entre 1 et " 
                              << mediaFiles.size() << std::endl;
                }
            } catch (const std::exception&) {
                std::cout << "❌ Entrée invalide. Tapez 'h' pour l'aide." << std::endl;
            }
        }
    }

    void ConsoleUI::launchExternalPlayer(const std::string &fileName) {
        std::string fullPath = "assets/" + fileName;
        
        // Essaie différents lecteurs
        std::vector<std::string> players = {"vlc", "mpv", "xdg-open", "mplayer"};
        
        std::cout << "🔍 Recherche d'un lecteur multimédia...\n";
        
        for (const auto& player : players) {
            std::string checkCmd = "which " + player + " > /dev/null 2>&1";
            if (system(checkCmd.c_str()) == 0) {
                std::cout << "✅ Lecteur trouvé: " << player << std::endl;
                std::string playCmd = player + " \"" + fullPath + "\" 2>/dev/null &";
                
                std::cout << "🚀 Commande: " << playCmd << std::endl;
                int result = system(playCmd.c_str());
                
                if (result == 0) {
                    std::cout << "▶️ Lecture lancée avec " << player << std::endl;
                    return;
                } else {
                    std::cout << "⚠️ Échec du lancement avec " << player << std::endl;
                }
            }
        }
        
        std::cout << "❌ Aucun lecteur multimédia trouvé!" << std::endl;
        std::cout << "💡 Installez VLC ou MPV: sudo apt install vlc mpv" << std::endl;
        std::cout << "   Ou utilisez l'interface Qt pour la lecture intégrée." << std::endl;
    }

    void ConsoleUI::showHelp() {
        std::cout << "\n📖 Commandes disponibles:\n";
        std::cout << "  [1-9]    : Lancer la lecture du fichier numéroté\n";
        std::cout << "  l, list  : Afficher la liste des fichiers\n";
        std::cout << "  h, help  : Afficher cette aide\n";
        std::cout << "  q, quit  : Quitter l'application\n";
    }
}