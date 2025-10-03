/*
** EPITECH PROJECT, 2025
** EpiKodi [WSL: Ubuntu]
** File description:
** media_controls - Composant réutilisable pour les contrôles de lecture multimédia
*/

#ifndef MEDIA_CONTROLS_HPP
#define MEDIA_CONTROLS_HPP

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <functional>

namespace epikodi {
    
    class MediaControls : public QWidget {
    public:
        explicit MediaControls(QWidget* parent = nullptr);
        virtual ~MediaControls() = default;
        
        // Setters pour les callbacks
        void setOnPlayPause(std::function<void()> callback);
        void setOnStop(std::function<void()> callback);
        void setOnRewind(std::function<void()> callback);
        void setOnFastForward(std::function<void()> callback);
        void setOnFullscreen(std::function<void()> callback);
        void setOnProgressChanged(std::function<void(int)> callback);
        void setOnVolumeChanged(std::function<void(int)> callback);
        
        // Méthodes publiques pour contrôler l'interface
        void setPlayPauseIcon(bool isPlaying);
        void setFullscreenIcon(bool isFullscreen);
        void setProgressValue(int value);
        void setProgressRange(int min, int max);
        void setTimeLabel(const QString& current, const QString& total);
        void setVolumeValue(int value);
        void enableControls(bool enabled);
        void setImageMode(bool imageMode);
        
    private:
        void setupUI();
        QString getButtonStyle();
        QString getSliderStyle();
        
        QHBoxLayout* layout;
        
        // Boutons de contrôle
        QPushButton* rewindButton;
        QPushButton* playPauseButton;
        QPushButton* stopButton;
        QPushButton* fastForwardButton;
        QPushButton* fullscreenButton;
        
        // Sliders
        QSlider* progressSlider;
        QSlider* volumeSlider;
        
        // Labels
        QLabel* timeLabel;
        QLabel* volumeLabel;
        
        // Callbacks
        std::function<void()> playPauseCallback;
        std::function<void()> stopCallback;
        std::function<void()> rewindCallback;
        std::function<void()> fastForwardCallback;
        std::function<void()> fullscreenCallback;
        std::function<void(int)> progressCallback;
        std::function<void(int)> volumeCallback;
    };
}

#endif // MEDIA_CONTROLS_HPP
