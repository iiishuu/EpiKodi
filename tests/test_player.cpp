/*
** EPITECH PROJECT, 2025  
** EpiKodi [WSL: Ubuntu]
** File description:
** test_player - Unit tests for media playback functionality
*/

#include "../include/media/player.hpp"
#include <iostream>
#include <cassert>
#include <fstream>
#include <filesystem>

// Helper to create test media files
void createTestMediaFiles() {
    // Create valid test files with proper content
    std::ofstream mp3("test_audio.mp3");
    mp3 << "ID3"; // MP3 header
    mp3 << std::string(100, 'X'); // Padding
    mp3.close();
    
    std::ofstream mp4("test_video.mp4");
    mp4 << "    ftyp"; // MP4 header
    mp4 << std::string(100, 'X'); // Padding
    mp4.close();
    
    std::ofstream wav("test_audio.wav");
    wav << std::string(100, 'W'); // WAV content
    wav.close();
}

void cleanupTestMediaFiles() {
    std::filesystem::remove("test_audio.mp3");
    std::filesystem::remove("test_video.mp4");
    std::filesystem::remove("test_audio.wav");
    std::filesystem::remove("corrupted.mp3");
}

void testPlayerStates() {
    epikodi::Player player;
    
    // Test initial state
    assert(player.getState() == epikodi::Player::State::STOPPED);
    
    // Test pause/resume cycle
    player.pause(); // Should handle pause when stopped gracefully
    assert(player.getState() == epikodi::Player::State::STOPPED);
    
    player.stop(); // Should handle stop when already stopped
    assert(player.getState() == epikodi::Player::State::STOPPED);
    
    std::cout << "✓ Player state management test passed" << std::endl;
}

void testAudioPlayback() {
    createTestMediaFiles();
    epikodi::Player player;
    
    std::string lastStatus;
    player.setStatusCallback([&lastStatus](const std::string& status) {
        lastStatus = status;
    });
    
    // Test MP3 playback
    player.play("test_audio.mp3");
    assert(player.getState() == epikodi::Player::State::PLAYING);
    assert(lastStatus.find("Audio playback started") != std::string::npos);
    
    // Test WAV playback  
    player.play("test_audio.wav");
    assert(player.getState() == epikodi::Player::State::PLAYING);
    assert(lastStatus.find("Audio playback started") != std::string::npos);
    
    cleanupTestMediaFiles();
    std::cout << "✓ Audio playback test passed" << std::endl;
}

void testVideoPlayback() {
    createTestMediaFiles();
    epikodi::Player player;
    
    std::string lastStatus;
    player.setStatusCallback([&lastStatus](const std::string& status) {
        lastStatus = status;
    });
    
    // Test MP4 playback
    player.play("test_video.mp4");
    assert(player.getState() == epikodi::Player::State::PLAYING);
    assert(lastStatus.find("Video playback started") != std::string::npos);
    
    cleanupTestMediaFiles();
    std::cout << "✓ Video playback test passed" << std::endl;
}

void testPlaybackControls() {
    createTestMediaFiles();
    epikodi::Player player;
    
    // Start playback
    player.play("test_audio.mp3");
    assert(player.getState() == epikodi::Player::State::PLAYING);
    
    // Test pause
    player.pause();
    assert(player.getState() == epikodi::Player::State::PAUSED);
    
    // Test resume
    player.pause(); // Should resume from paused
    assert(player.getState() == epikodi::Player::State::PLAYING);
    
    // Test stop
    player.stop();
    assert(player.getState() == epikodi::Player::State::STOPPED);
    
    // Test next
    player.next(); // Should handle gracefully without playlist
    
    cleanupTestMediaFiles();
    std::cout << "✓ Playback controls test passed" << std::endl;
}

void testErrorHandling() {
    epikodi::Player player;
    
    std::string lastError;
    player.setErrorCallback([&lastError](const std::string& error) {
        lastError = error;
    });
    
    // Test file not found
    player.play("nonexistent.mp3");
    assert(player.getState() == epikodi::Player::State::ERROR);
    assert(lastError.find("File not found") != std::string::npos);
    
    // Test corrupted file
    std::ofstream corrupted("corrupted.mp3");
    corrupted << "x"; // Too small
    corrupted.close();
    
    player.play("corrupted.mp3");
    assert(player.getState() == epikodi::Player::State::ERROR);
    assert(lastError.find("File corrupted") != std::string::npos);
    
    // Test unsupported format
    player.play("document.txt");
    assert(player.getState() == epikodi::Player::State::ERROR);
    assert(lastError.find("Unsupported format") != std::string::npos);
    
    cleanupTestMediaFiles();
    std::cout << "✓ Error handling test passed" << std::endl;
}

void testFormatSupport() {
    epikodi::Player player;
    
    // Test supported audio formats
    assert(player.canPlayFile("test.mp3") == true);
    assert(player.canPlayFile("test.wav") == true);
    assert(player.canPlayFile("test.ogg") == true);
    assert(player.canPlayFile("song.MP3") == true); // Case insensitive
    
    // Test supported video formats  
    assert(player.canPlayFile("movie.mp4") == true);
    assert(player.canPlayFile("video.avi") == true);
    assert(player.canPlayFile("film.mkv") == true);
    
    // Test unsupported formats
    assert(player.canPlayFile("document.txt") == false);
    assert(player.canPlayFile("archive.zip") == false);
    assert(player.canPlayFile("image.jpg") == false); // Images not playable
    
    std::cout << "✓ Format support test passed" << std::endl;
}

int main() {
    try {
        std::cout << "Testing Player playback functionality..." << std::endl;
        
        testPlayerStates();
        testAudioPlayback();
        testVideoPlayback();
        testPlaybackControls();
        testErrorHandling();
        testFormatSupport();
        
        std::cout << "\n🎵 All playback tests passed! 🎵" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        cleanupTestMediaFiles();
        return 1;
    }
}