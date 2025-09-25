/*
** EPITECH PROJECT, 2025
** EpiKodi [WSL: Ubuntu]
** File description:
** main_console - Version console simple
*/

#include "ui/console_ui.hpp"
#include "media/folder_parser.hpp"
#include <iostream>

int main(int argc, char **argv) {
    (void)argc; // Évite les warnings unused
    (void)argv;
    
    std::cout << "🎬 EpiKodi Console - Démarrage..." << std::endl;
    
    epikodi::ConsoleUI ui;
    ui.openWindow("EpiKodi Console");

    auto files = epikodi::FolderParser::listFiles("assets");
    
    // Création de la liste des infos détaillées
    std::vector<std::string> fileInfos;
    for (const auto &f : files) {
        fileInfos.push_back(f.getInfo());
    }

    std::cout << "🚀 Interface console lancée avec " << files.size() << " fichier(s)" << std::endl;
    
    ui.showMenu(fileInfos);
    ui.closeWindow();

    return 0;
}