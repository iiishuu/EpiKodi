/*
** EPITECH PROJECT, 2025
** EpiKodi [WSL: Ubuntu]
** File description:
** test_parser - Simple unit tests for file detection
*/

#include "../include/media/folder_parser.hpp"
#include "../include/media/media_file.hpp"
#include <iostream>
#include <cassert>

void testBasicParsing() {
    // Test that parser can handle assets directory
    auto mediaFiles = epikodi::FolderParser::listFiles("assets");
    
    // Should find some files in assets
    assert(mediaFiles.size() > 0);
    assert(epikodi::FolderParser::hasMediaFiles("assets") == true);
    
    std::cout << "✓ Found " << mediaFiles.size() << " media files in assets" << std::endl;
}

void testMediaTypeDetection() {
    // Test static method for type detection
    assert(epikodi::MediaFile::determineMediaType(".mp4") == epikodi::MediaType::VIDEO);
    assert(epikodi::MediaFile::determineMediaType(".mp3") == epikodi::MediaType::AUDIO);
    assert(epikodi::MediaFile::determineMediaType(".png") == epikodi::MediaType::IMAGE);
    assert(epikodi::MediaFile::determineMediaType(".txt") == epikodi::MediaType::UNKNOWN);
    
    std::cout << "✓ Media type detection test passed" << std::endl;
}

int main() {
    try {
        std::cout << "Testing parsing functionality..." << std::endl;
        
        testBasicParsing();
        testMediaTypeDetection();
        
        std::cout << "\n📁 All parsing tests passed! 📁" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }
}