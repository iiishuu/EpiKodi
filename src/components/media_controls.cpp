/*
** EPITECH PROJECT, 2025
** EpiKodi [WSL: Ubuntu]
** File description:
** media_controls - Implémentation du composant contrôles multimédia
*/

#include "components/media_controls.hpp"

namespace epikodi {
    
    MediaControls::MediaControls(QWidget* parent)
        : QWidget(parent) {
        setupUI();
    }
    
    void MediaControls::setupUI() {
        layout = new QHBoxLayout(this);
        layout->setSpacing(10);
        layout->setContentsMargins(0, 0, 0, 0);
        
        // Boutons de contrôle
        rewindButton = new QPushButton("⏪");
        playPauseButton = new QPushButton("▶️");
        stopButton = new QPushButton("⏹️");
        fastForwardButton = new QPushButton("⏩");
        
        rewindButton->setToolTip("Reculer de 5 secondes");
        playPauseButton->setToolTip("Lecture/Pause");
        stopButton->setToolTip("Arrêter");
        fastForwardButton->setToolTip("Avancer de 5 secondes");
        
        QString buttonStyle = getButtonStyle();
        rewindButton->setStyleSheet(buttonStyle);
        playPauseButton->setStyleSheet(buttonStyle);
        stopButton->setStyleSheet(buttonStyle);
        fastForwardButton->setStyleSheet(buttonStyle);
        
        rewindButton->setFixedSize(45, 35);
        playPauseButton->setFixedSize(45, 35);
        stopButton->setFixedSize(45, 35);
        fastForwardButton->setFixedSize(45, 35);
        
        connect(rewindButton, &QPushButton::clicked, [this]() {
            if (rewindCallback) rewindCallback();
        });
        connect(playPauseButton, &QPushButton::clicked, [this]() {
            if (playPauseCallback) playPauseCallback();
        });
        connect(stopButton, &QPushButton::clicked, [this]() {
            if (stopCallback) stopCallback();
        });
        connect(fastForwardButton, &QPushButton::clicked, [this]() {
            if (fastForwardCallback) fastForwardCallback();
        });
        
        // Slider de progression
        progressSlider = new QSlider(Qt::Horizontal);
        progressSlider->setRange(0, 100);
        progressSlider->setValue(0);
        progressSlider->setStyleSheet(getSliderStyle());
        
        connect(progressSlider, &QSlider::sliderMoved, [this](int position) {
            if (progressCallback) progressCallback(position);
        });
        
        // Label de temps
        timeLabel = new QLabel("00:00 / 00:00");
        timeLabel->setStyleSheet(
            "color: rgba(255, 255, 255, 200); "
            "font-size: 11px; "
            "background: transparent; "
            "border: none;"
        );
        timeLabel->setFixedWidth(80);
        
        // Bouton plein écran
        fullscreenButton = new QPushButton("⛶");
        fullscreenButton->setToolTip("Plein écran");
        fullscreenButton->setStyleSheet(buttonStyle);
        fullscreenButton->setFixedSize(45, 35);
        
        connect(fullscreenButton, &QPushButton::clicked, [this]() {
            if (fullscreenCallback) fullscreenCallback();
        });
        
        // Contrôle du volume
        volumeLabel = new QLabel("🔊");
        volumeLabel->setStyleSheet(
            "color: white; "
            "font-size: 14px; "
            "background: transparent; "
            "border: none;"
        );
        
        volumeSlider = new QSlider(Qt::Horizontal);
        volumeSlider->setRange(0, 100);
        volumeSlider->setValue(50);
        volumeSlider->setFixedWidth(80);
        volumeSlider->setStyleSheet(getSliderStyle());
        
        connect(volumeSlider, &QSlider::valueChanged, [this](int value) {
            QString volumeText = (value == 0) ? "🔇" : (value < 30) ? "🔉" : "🔊";
            volumeLabel->setText(volumeText);
            if (volumeCallback) volumeCallback(value);
        });
        
        // Layout des contrôles
        layout->addWidget(rewindButton);
        layout->addWidget(playPauseButton);
        layout->addWidget(stopButton);
        layout->addWidget(fastForwardButton);
        layout->addWidget(progressSlider, 1);
        layout->addWidget(timeLabel);
        layout->addWidget(fullscreenButton);
        layout->addWidget(volumeLabel);
        layout->addWidget(volumeSlider);
    }
    
    QString MediaControls::getButtonStyle() {
        return "QPushButton { "
               "  background: rgba(75, 85, 99, 200); "
               "  border: 1px solid rgba(107, 114, 128, 150); "
               "  border-radius: 6px; "
               "  color: white; "
               "  font-size: 14px; "
               "  font-weight: 600; "
               "} "
               "QPushButton:hover { "
               "  background: rgba(107, 114, 128, 230); "
               "} "
               "QPushButton:pressed { "
               "  background: rgba(55, 65, 81, 200); "
               "}";
    }
    
    QString MediaControls::getSliderStyle() {
        return "QSlider::groove:horizontal { "
               "  border: 1px solid rgba(60, 70, 90, 200); "
               "  height: 6px; "
               "  background: rgba(30, 40, 60, 200); "
               "  border-radius: 3px; "
               "} "
               "QSlider::handle:horizontal { "
               "  background: rgba(59, 130, 246, 255); "
               "  border: 2px solid rgba(79, 150, 255, 255); "
               "  width: 16px; "
               "  height: 16px; "
               "  border-radius: 8px; "
               "  margin: -5px 0; "
               "} "
               "QSlider::sub-page:horizontal { "
               "  background: rgba(59, 130, 246, 180); "
               "  border-radius: 3px; "
               "}";
    }
    
    // Setters pour les callbacks
    void MediaControls::setOnPlayPause(std::function<void()> callback) {
        playPauseCallback = callback;
    }
    
    void MediaControls::setOnStop(std::function<void()> callback) {
        stopCallback = callback;
    }
    
    void MediaControls::setOnRewind(std::function<void()> callback) {
        rewindCallback = callback;
    }
    
    void MediaControls::setOnFastForward(std::function<void()> callback) {
        fastForwardCallback = callback;
    }
    
    void MediaControls::setOnFullscreen(std::function<void()> callback) {
        fullscreenCallback = callback;
    }
    
    void MediaControls::setOnProgressChanged(std::function<void(int)> callback) {
        progressCallback = callback;
    }
    
    void MediaControls::setOnVolumeChanged(std::function<void(int)> callback) {
        volumeCallback = callback;
    }
    
    // Méthodes pour contrôler l'interface
    void MediaControls::setPlayPauseIcon(bool isPlaying) {
        playPauseButton->setText(isPlaying ? "⏸️" : "▶️");
    }
    
    void MediaControls::setFullscreenIcon(bool isFullscreen) {
        fullscreenButton->setText(isFullscreen ? "⛶" : "⛶");
        fullscreenButton->setToolTip(isFullscreen ? "Quitter le plein écran" : "Plein écran");
    }
    
    void MediaControls::setProgressValue(int value) {
        progressSlider->blockSignals(true);
        progressSlider->setValue(value);
        progressSlider->blockSignals(false);
    }
    
    void MediaControls::setProgressRange(int min, int max) {
        progressSlider->setRange(min, max);
    }
    
    void MediaControls::setTimeLabel(const QString& current, const QString& total) {
        timeLabel->setText(current + " / " + total);
    }
    
    void MediaControls::setVolumeValue(int value) {
        volumeSlider->setValue(value);
    }
    
    void MediaControls::enableControls(bool enabled) {
        rewindButton->setEnabled(enabled);
        playPauseButton->setEnabled(enabled);
        stopButton->setEnabled(enabled);
        fastForwardButton->setEnabled(enabled);
        fullscreenButton->setEnabled(enabled);
        progressSlider->setEnabled(enabled);
        volumeSlider->setEnabled(enabled);
    }
    
    void MediaControls::setImageMode(bool imageMode) {
        if (imageMode) {
            // Pour les images, cacher tous les contrôles sauf plein écran
            rewindButton->hide();
            playPauseButton->hide();
            stopButton->hide();
            fastForwardButton->hide();
            progressSlider->hide();
            timeLabel->hide();
            volumeLabel->hide();
            volumeSlider->hide();
            fullscreenButton->show();
        } else {
            // Pour vidéo/audio, afficher tous les contrôles
            rewindButton->show();
            playPauseButton->show();
            stopButton->show();
            fastForwardButton->show();
            progressSlider->show();
            timeLabel->show();
            volumeLabel->show();
            volumeSlider->show();
            fullscreenButton->show();
        }
    }
}
