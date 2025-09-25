/*
** EPITECH PROJECT, 2025
** EpiKodi [WSL: Ubuntu]
** File description:
** app
*/

#ifndef APP_HPP
#define APP_HPP

#include <string>

namespace epikodi {
    class App {
    public:
        App() = default;
        void run(const std::string &mediaPath);
    };
}

#endif // APP_HPP
