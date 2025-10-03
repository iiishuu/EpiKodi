/*
** EPITECH PROJECT, 2025
** EpiKodi [WSL: Ubuntu]
** File description:
** fullscreen_page - Page plein écran pour la lecture de médias
*/

#ifndef FULLSCREEN_PAGE_HPP
#define FULLSCREEN_PAGE_HPP

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <functional>
#include "components/media_controls.hpp"
#include "components/file_explorer.hpp"

namespace epikodi {
    
    class FullscreenPage : public QWidget {
    public:
        explicit FullscreenPage(QWidget* parent = nullptr);
        virtual ~FullscreenPage() = default;
        
        void setOnBack(std::function<void()> callback);
        void loadMedia(const QString& filePath, MediaType type, QMediaPlayer* sourcePlayer);
        
        // Récupérer l'état actuel pour le transférer
        qint64 getCurrentPosition() const;
        int getCurrentVolume() const;
        bool getIsPlaying() const;
        void stopPlayer();
        
    protected:
        void mouseMoveEvent(QMouseEvent* event) override;
        void keyPressEvent(QKeyEvent* event) override;
        void resizeEvent(QResizeEvent* event) override;
        
    private:
        void setupUI();
        void showControls();
        void hideControls();
        void onControlsTimeout();
        
        MediaType mediaType;
        QString currentMediaPath;
        bool isPlaying;
        bool controlsVisible;
        
        QVBoxLayout* mainLayout;
        
        // Zone d'affichage
        QWidget* displayContainer;
        QLabel* mediaDisplay;
        QVideoWidget* videoWidget;
        
        // Lecteur multimédia
        QMediaPlayer* player;
        
        // Contrôles
        MediaControls* controls;
        QWidget* controlsContainer;
        
        // Timer pour masquer les contrôles
        QTimer* hideControlsTimer;
        
        std::function<void()> backCallback;
    };
}

#endif // FULLSCREEN_PAGE_HPP
