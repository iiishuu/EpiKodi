/*
** EPITECH PROJECT, 2025  
** EpiKodi [WSL: Ubuntu]
** File description:
** test_player
*/

#include "../include/media/player.hpp"
#include <iostream>
#include <cassert>

void testPlayerStates() {
    epikodi::Player player;
    
    // Test initial state
    assert(player.getState() == epikodi::Player::State::STOPPED);
    
    // Test pause/resume
    player.pause(); // Should handle pause when stopped
    player.stop();
    
    std::cout << "✓ Player states test passed" << std::endl;
}

void testFormatSupport() {
    epikodi::Player player;
    
    // Test audio formats
    assert(player.canPlayFile("test.mp3") == true);
    assert(player.canPlayFile("test.wav") == true);
    
    // Test video formats  
    assert(player.canPlayFile("movie.mp4") == true);
    
    // Test unsupported formats
    assert(player.canPlayFile("document.txt") == false);
    
    std::cout << "✓ Format support test passed" << std::endl;
}

int main() {
    try {
        std::cout << "Testing Player class..." << std::endl;
        
        testPlayerStates();
        testFormatSupport();
        
        std::cout << "\n🎵 All Player tests passed! 🎵" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }
}