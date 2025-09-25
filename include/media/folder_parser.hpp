/*
** EPITECH PROJECT, 2025
** EpiKodi [WSL: Ubuntu]
** File description:
** folder_parser
*/

#ifndef FOLDER_PARSER_HPP
#define FOLDER_PARSER_HPP

#include "media/media_file.hpp"
#include <string>
#include <vector>

namespace epikodi {
    class FolderParser {
    public:
        static std::vector<MediaFile> listFiles(const std::string &path);
        static std::vector<MediaFile> listFilesByType(const std::string &path, MediaType type);
        static bool hasMediaFiles(const std::string &path);
    };
}

#endif // FOLDER_PARSER_HPP
