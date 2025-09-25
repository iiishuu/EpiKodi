/*
** EPITECH PROJECT, 2025
** EpiKodi [WSL: Ubuntu]
** File description:
** ui_manager
*/

#ifndef UI_MANAGER_HPP
#define UI_MANAGER_HPP

#include <string>
#include <vector>

namespace epikodi {
    class UIManager {
    public:
        virtual ~UIManager() = default;

        virtual void openWindow(const std::string &title) = 0;
        virtual void closeWindow() = 0;
        virtual void showMenu(const std::vector<std::string> &mediaFiles) = 0;
    };
}

#endif // UI_MANAGER_HPP
