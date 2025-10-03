/*
** EPITECH PROJECT, 2025
** EpiKodi [WSL: Ubuntu]
** File description:
** main_window - Implémentation de la fenêtre principale
*/

#include "ui/main_window.hpp"
#include "ui/menu_page.hpp"
#include "ui/video_page.hpp"
#include "ui/image_page.hpp"
#include "ui/audio_page.hpp"
#include "ui/settings_page.hpp"
#include "ui/fullscreen_page.hpp"

namespace epikodi {
    
    MainWindow::MainWindow(QWidget* parent) 
        : QMainWindow(parent), previousPage(nullptr) {
        setupUI();
        setupPages();
        setupStyle();
        showMenuPage();
    }
    
    MainWindow::~MainWindow() = default;
    
    void MainWindow::setupUI() {
        setWindowTitle("EpiKodi v2.0 - Centre Multimédia");
        resize(1200, 800);
        
        stackedWidget = new QStackedWidget();
        setCentralWidget(stackedWidget);
    }
    
    void MainWindow::setupPages() {
        menuPage = std::make_unique<MenuPage>();
        videoPage = std::make_unique<VideoPage>();
        imagePage = std::make_unique<ImagePage>();
        audioPage = std::make_unique<AudioPage>();
        settingsPage = std::make_unique<SettingsPage>();
        fullscreenPage = std::make_unique<FullscreenPage>();
        
        stackedWidget->addWidget(menuPage.get());
        stackedWidget->addWidget(videoPage.get());
        stackedWidget->addWidget(imagePage.get());
        stackedWidget->addWidget(audioPage.get());
        stackedWidget->addWidget(settingsPage.get());
        stackedWidget->addWidget(fullscreenPage.get());
        
        // Configuration des callbacks de navigation
        menuPage->setOnVideoRequested([this]() { showVideoPage(); });
        menuPage->setOnImageRequested([this]() { showImagePage(); });
        menuPage->setOnAudioRequested([this]() { showAudioPage(); });
        menuPage->setOnSettingsRequested([this]() { showSettingsPage(); });
        
        // Boutons retour pour toutes les pages
        videoPage->setOnBackRequested([this]() { showMenuPage(); });
        imagePage->setOnBackRequested([this]() { showMenuPage(); });
        audioPage->setOnBackRequested([this]() { showMenuPage(); });
        settingsPage->setOnBackRequested([this]() { showMenuPage(); });
        
        // Connexions pour le plein écran
        videoPage->setOnFullscreenRequested([this](const QString& path, MediaType type, QMediaPlayer* player) {
            showFullscreenPage(path, type, player);
        });
        audioPage->setOnFullscreenRequested([this](const QString& path, MediaType type, QMediaPlayer* player) {
            showFullscreenPage(path, type, player);
        });
        imagePage->setOnFullscreenRequested([this](const QString& path, MediaType type, QMediaPlayer* player) {
            showFullscreenPage(path, type, player);
        });
        
        fullscreenPage->setOnBack([this]() {
            returnFromFullscreen();
        });
    }
    
    void MainWindow::setupStyle() {
        setStyleSheet(
            "QMainWindow { "
            "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
            "    stop:0 rgba(25, 35, 50, 255), stop:0.5 rgba(20, 30, 45, 255), "
            "    stop:1 rgba(15, 25, 40, 255)); "
            "}"
        );
    }
    
    void MainWindow::showMenuPage() {
        stackedWidget->setCurrentWidget(menuPage.get());
    }
    
    void MainWindow::showVideoPage() {
        stackedWidget->setCurrentWidget(videoPage.get());
    }
    
    void MainWindow::showImagePage() {
        stackedWidget->setCurrentWidget(imagePage.get());
    }
    
    void MainWindow::showAudioPage() {
        stackedWidget->setCurrentWidget(audioPage.get());
    }
    
    void MainWindow::showSettingsPage() {
        stackedWidget->setCurrentWidget(settingsPage.get());
    }
    
    void MainWindow::showFullscreenPage(const QString& filePath, MediaType type, QMediaPlayer* sourcePlayer) {
        // Sauvegarder la page actuelle
        previousPage = stackedWidget->currentWidget();
        
        fullscreenPage->loadMedia(filePath, type, sourcePlayer);
        stackedWidget->setCurrentWidget(fullscreenPage.get());
        fullscreenPage->setFocus();
    }
    
    void MainWindow::returnFromFullscreen() {
        // Récupérer l'état du lecteur fullscreen
        qint64 position = fullscreenPage->getCurrentPosition();
        int volume = fullscreenPage->getCurrentVolume();
        bool wasPlaying = fullscreenPage->getIsPlaying();
        
        // Arrêter le lecteur fullscreen
        fullscreenPage->stopPlayer();
        
        // Retourner à la page sauvegardée
        if (previousPage) {
            stackedWidget->setCurrentWidget(previousPage);
            
            // Restaurer l'état dans le MediaPlayer de la page d'origine
            if (previousPage == videoPage.get()) {
                videoPage->getMediaPlayer()->restorePlayerState(position, volume, wasPlaying);
            } else if (previousPage == audioPage.get()) {
                audioPage->getMediaPlayer()->restorePlayerState(position, volume, wasPlaying);
            } else if (previousPage == imagePage.get()) {
                imagePage->getMediaPlayer()->restorePlayerState(position, volume, wasPlaying);
            }
            
            previousPage = nullptr;
        } else {
            showMenuPage();
        }
    }
}