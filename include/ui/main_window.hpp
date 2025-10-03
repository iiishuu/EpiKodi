/*
** EPITECH PROJECT, 2025
** EpiKodi [WSL: Ubuntu]
** File description:
** main_window - Fenêtre principale avec navigation
*/

#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <QMainWindow>
#include <QStackedWidget>
#include <QMediaPlayer>
#include <memory>
#include "components/file_explorer.hpp"

namespace epikodi {
    class MenuPage;
    class VideoPage;
    class ImagePage;
    class AudioPage;
    class SettingsPage;
    class FullscreenPage;
    
    class MainWindow : public QMainWindow {
    public:
        explicit MainWindow(QWidget* parent = nullptr);
        ~MainWindow();
        
        void showMenuPage();
        void showVideoPage();
        void showImagePage();
        void showAudioPage();
        void showSettingsPage();
        void showFullscreenPage(const QString& filePath, MediaType type, QMediaPlayer* sourcePlayer);
        void returnFromFullscreen();
        
    private:
        void setupUI();
        void setupPages();
        void setupStyle();
        
        QStackedWidget* stackedWidget;
        std::unique_ptr<MenuPage> menuPage;
        std::unique_ptr<VideoPage> videoPage;
        std::unique_ptr<ImagePage> imagePage;
        std::unique_ptr<AudioPage> audioPage;
        std::unique_ptr<SettingsPage> settingsPage;
        std::unique_ptr<FullscreenPage> fullscreenPage;
        
        QWidget* previousPage;
    };
}

#endif // MAIN_WINDOW_HPP