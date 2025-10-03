/*
** EPITECH PROJECT, 2025
** EpiKodi [WSL: Ubuntu]
** File description:
** application - Point d'entrée principal de l'application
*/

#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <QApplication>
#include <memory>

namespace epikodi {
    class MainWindow;
    
    class Application {
    public:
        Application(int& argc, char** argv);
        ~Application();
        
        int run();
        void quit();
        
    private:
        QApplication app;
        std::unique_ptr<MainWindow> window;
    };
}

#endif // APPLICATION_HPP