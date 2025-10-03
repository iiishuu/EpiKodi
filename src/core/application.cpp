/*
** EPITECH PROJECT, 2025
** EpiKodi [WSL: Ubuntu]
** File description:
** application - Implémentation du point d'entrée principal
*/

#include "core/application.hpp"
#include "ui/main_window.hpp"

namespace epikodi {
    
    Application::Application(int& argc, char** argv) 
        : app(argc, argv) {
        window = std::make_unique<MainWindow>();
    }
    
    Application::~Application() = default;
    
    int Application::run() {
        window->show();
        return app.exec();
    }
    
    void Application::quit() {
        app.quit();
    }
}