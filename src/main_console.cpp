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
    
    std::cout << "EpiKodi Console - Demarrage..." << std::endl;
    
    epikodi::ConsoleUI ui;
    ui.openWindow("EpiKodi Console");

    auto files = epikodi::FolderParser::listFiles("assets");

    std::cout << "Interface console lancee avec " << files.size() << " fichier(s)" << std::endl;
    
    ui.showMenu(files);
    ui.closeWindow();

    return 0;
}