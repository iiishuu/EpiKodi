/*
** EPITECH PROJECT, 2025
** EpiKodi [WSL: Ubuntu]
** File description:
** main
*/

#include "core/app.hpp"
#include "ui/qt_ui.hpp"
#include "media/folder_parser.hpp"
#include <iostream>

int main(int argc, char **argv) {
    std::cout << "🎬 EpiKodi - Démarrage..." << std::endl;
    
    // Vérification du dossier assets
    if (!epikodi::FolderParser::hasMediaFiles("assets")) {
        std::cout << "⚠️  Aucun fichier média trouvé dans le dossier 'assets/'" << std::endl;
        std::cout << "💡 Placez vos fichiers vidéo, audio ou image dans ce dossier." << std::endl;
    }
    
    epikodi::QtUI ui(argc, argv);
    ui.openWindow("🎬 EpiKodi - Media Center");

    auto files = epikodi::FolderParser::listFiles("assets");
    
    // On passe les vrais noms de fichiers, pas les infos formatées
    std::vector<std::string> fileNames;
    for (const auto &f : files) {
        fileNames.push_back(f.getName());
    }

    std::cout << "🚀 Interface lancée avec " << files.size() << " fichier(s)" << std::endl;
    
    ui.showMenu(fileNames);

    return 0;
}

