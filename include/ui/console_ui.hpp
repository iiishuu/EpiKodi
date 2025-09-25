/*
** EPITECH PROJECT, 2025
** EpiKodi [WSL: Ubuntu]
** File description:
** console_ui
*/

#ifndef CONSOLE_UI_HPP
#define CONSOLE_UI_HPP

#include "ui/ui_manager.hpp"
#include <string>
#include <vector>

namespace epikodi {
    class ConsoleUI : public UIManager {
    public:
        ConsoleUI();
        ~ConsoleUI();

        void openWindow(const std::string &title) override;
        void closeWindow() override;
        void showMenu(const std::vector<std::string> &mediaFiles) override;

    private:
        void printHeader(const std::string &title);
        void printMediaList(const std::vector<std::string> &mediaFiles);
        void handleUserInput(const std::vector<std::string> &mediaFiles);
        void launchExternalPlayer(const std::string &fileName);
        void showHelp();
    };
}

#endif // CONSOLE_UI_HPP