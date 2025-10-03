/*
** EPITECH PROJECT, 2025
** EpiKodi [WSL: Ubuntu]
** File description:
** main - Point d'entrée propre et simple
*/

#include <QApplication>
#include "ui/main_window.hpp"

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    
    epikodi::MainWindow window;
    window.show();
    
    return app.exec();
}