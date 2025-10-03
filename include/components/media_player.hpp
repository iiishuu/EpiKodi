/*
** EPITECH PROJECT, 2025
** EpiKodi [WSL: Ubuntu]
** File description:
** media_player - Composant lecteur de médias (à droite)
*/

#ifndef MEDIA_PLAYER_HPP
#define MEDIA_PLAYER_HPP

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QFileInfo>
#include <QPixmap>
#include <QScrollArea>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <functional>
#include "components/file_explorer.hpp"
#include "components/media_controls.hpp"

namespace epikodi {
    
    class MediaPlayer : public QWidget {
        // Q_OBJECT temporairement commenté
        
    public:
        explicit MediaPlayer(MediaType type, QWidget* parent = nullptr);
        virtual ~MediaPlayer() = default;
        
        void loadMedia(const QString& filePath);
        void playMedia();
        void pauseMedia();
        void stopMedia();
        void setVolume(int volume);
        void rewind5Seconds();
        void fastForward5Seconds();
        void toggleFullscreen();
        
        // Restaurer l'état du lecteur
        void restorePlayerState(qint64 position, int volume, bool shouldPlay);
        
        // Setter pour la callback plein écran
        void setOnFullscreenRequested(std::function<void(const QString&, MediaType, QMediaPlayer*)> callback);
        
        // Méthodes publiques (anciennement slots)
        void onPlayPause();
        void onStop();
        void onVolumeChanged(int value);
        void updateProgress();
        void onPositionChanged(qint64 position);
        void onDurationChanged(qint64 duration);
        void onMediaStatusChanged(QMediaPlayer::MediaStatus status);
        void onProgressSliderMoved(int position);
        
    private:
        void setupUI();
        void setupMediaDisplay();
        void displayImage(const QString& filePath);
        void displayAudioInfo(const QString& filePath);
        QString getPlayerStyle();
        QString formatTime(qint64 milliseconds);
        
        MediaType mediaType;
        QString currentMediaPath;
        bool isPlaying;
        bool isFullscreen;
        
        QVBoxLayout* mainLayout;
        
        // Zone d'affichage
        QScrollArea* displayArea;
        QWidget* displayContainer;
        QLabel* mediaDisplay;
        QVideoWidget* videoWidget;
        QLabel* mediaInfoLabel;
        
        // Lecteur multimédia
        QMediaPlayer* player;
        
        // Contrôles (composant réutilisable)
        MediaControls* controls;
        
        QTimer* progressTimer;
        
        // Callback pour le plein écran
        std::function<void(const QString&, MediaType, QMediaPlayer*)> fullscreenCallback;
    };
}

#endif // MEDIA_PLAYER_HPP