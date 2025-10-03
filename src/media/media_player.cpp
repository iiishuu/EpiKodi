/*
** EPITECH PROJECT, 2025
** EpiKodi [WSL: Ubuntu]
** File description:
** media_player - Implémentation du composant lecteur de médias
*/

#include "media/media_player.hpp"
#include <QFileInfo>
#include <QPixmap>
#include <QImageReader>

namespace epikodi {
    
    MediaPlayer::MediaPlayer(MediaType type, QWidget* parent)
        : QWidget(parent), mediaType(type), isPlaying(false), isFullscreen(false) {
        
        // Initialisation du lecteur multimédia (Qt5)
        player = new QMediaPlayer(this);
        
        // Connexions pour les événements du player (Qt5)
        connect(player, &QMediaPlayer::positionChanged, [this](qint64 position) {
            onPositionChanged(position);
        });
        connect(player, &QMediaPlayer::durationChanged, [this](qint64 duration) {
            onDurationChanged(duration);
        });
        connect(player, &QMediaPlayer::mediaStatusChanged, [this](QMediaPlayer::MediaStatus status) {
            onMediaStatusChanged(status);
        });
        
        progressTimer = new QTimer(this);
        connect(progressTimer, &QTimer::timeout, [this]() { updateProgress(); });
        
        setupUI();
        setupMediaDisplay();
        
        // Initialiser le composant MediaControls
        controls = new MediaControls(this);
        mainLayout->addWidget(controls);
        
        // Connecter les callbacks du composant MediaControls
        controls->setOnPlayPause([this]() { onPlayPause(); });
        controls->setOnStop([this]() { onStop(); });
        controls->setOnRewind([this]() { rewind5Seconds(); });
        controls->setOnFastForward([this]() { fastForward5Seconds(); });
        controls->setOnFullscreen([this]() { toggleFullscreen(); });
        controls->setOnProgressChanged([this](int position) { onProgressSliderMoved(position); });
        controls->setOnVolumeChanged([this](int value) { onVolumeChanged(value); });
        
        // Initialiser le volume
        controls->setVolumeValue(50);
        player->setVolume(50);
    }
    
    void MediaPlayer::setupUI() {
        mainLayout = new QVBoxLayout(this);
        mainLayout->setSpacing(15);
        mainLayout->setContentsMargins(20, 20, 20, 20);
        
        setStyleSheet(getPlayerStyle());
    }
    
    void MediaPlayer::setupMediaDisplay() {
        // Zone d'affichage du média
        displayArea = new QScrollArea();
        displayArea->setWidgetResizable(true);
        displayArea->setAlignment(Qt::AlignCenter);
        displayArea->setStyleSheet(
            "QScrollArea { "
            "  background: rgba(5, 10, 20, 200); "
            "  border: 2px solid rgba(40, 50, 70, 150); "
            "  border-radius: 8px; "
            "} "
            "QScrollBar:vertical, QScrollBar:horizontal { "
            "  background: rgba(30, 40, 60, 150); "
            "  width: 12px; height: 12px; "
            "  border-radius: 6px; "
            "} "
            "QScrollBar::handle { "
            "  background: rgba(75, 85, 99, 200); "
            "  border-radius: 6px; "
            "  min-height: 20px; min-width: 20px; "
            "} "
            "QScrollBar::handle:hover { "
            "  background: rgba(107, 114, 128, 200); "
            "}"
        );
        
        // Container pour le contenu (vidéo ou image/label)
        displayContainer = new QWidget();
        QVBoxLayout* containerLayout = new QVBoxLayout(displayContainer);
        containerLayout->setContentsMargins(0, 0, 0, 0);
        
        // Label pour images et messages
        mediaDisplay = new QLabel();
        mediaDisplay->setAlignment(Qt::AlignCenter);
        mediaDisplay->setMinimumSize(400, 300);
        mediaDisplay->setStyleSheet(
            "background: transparent; "
            "color: rgba(255, 255, 255, 150); "
            "font-size: 16px; "
            "border: none;"
        );
        mediaDisplay->setText("Aucun média sélectionné\\n\\nUtilisez l'explorateur pour charger un fichier");
        
        // Widget vidéo
        videoWidget = new QVideoWidget();
        videoWidget->setMinimumSize(400, 300);
        videoWidget->setStyleSheet("background: black;");
        videoWidget->hide();
        
        containerLayout->addWidget(mediaDisplay);
        containerLayout->addWidget(videoWidget);
        
        displayArea->setWidget(displayContainer);
        mainLayout->addWidget(displayArea, 1);
        
        // Informations du média
        mediaInfoLabel = new QLabel();
        mediaInfoLabel->setStyleSheet(
            "font-size: 12px; "
            "color: rgba(255, 255, 255, 180); "
            "background: rgba(20, 30, 50, 150); "
            "padding: 8px; "
            "border-radius: 5px; "
            "border: none;"
        );
        mediaInfoLabel->hide();
        mainLayout->addWidget(mediaInfoLabel);
    }
    
    QString MediaPlayer::getPlayerStyle() {
        return "background: rgba(15, 20, 30, 230); "
               "border: 1px solid rgba(50, 60, 80, 180); "
               "border-radius: 12px;";
    }
    
    void MediaPlayer::loadMedia(const QString& filePath) {
        if (filePath.isEmpty()) return;
        
        currentMediaPath = filePath;
        QFileInfo fileInfo(filePath);
        
        // Arrêter la lecture en cours (Qt5)
        if (player->state() == QMediaPlayer::PlayingState) {
            player->stop();
        }
        
        // Affichage des informations du fichier
        QString info = QString("📁 %1\\n📏 %2 KB\\n🕒 %3")
                      .arg(fileInfo.fileName())
                      .arg(fileInfo.size() / 1024)
                      .arg(fileInfo.lastModified().toString("dd/MM/yyyy hh:mm"));
        mediaInfoLabel->setText(info);
        mediaInfoLabel->show();
        
        // Configuration selon le type de média (Qt5)
        switch (mediaType) {
            case MediaType::VIDEO:
                // Afficher le widget vidéo
                mediaDisplay->hide();
                videoWidget->show();
                player->setVideoOutput(videoWidget);
                player->setMedia(QUrl::fromLocalFile(filePath));
                break;
                
            case MediaType::AUDIO:
                // Masquer le widget vidéo, afficher les infos audio
                videoWidget->hide();
                mediaDisplay->show();
                displayAudioInfo(filePath);
                player->setMedia(QUrl::fromLocalFile(filePath));
                break;
                
            case MediaType::IMAGE:
                // Afficher l'image
                videoWidget->hide();
                mediaDisplay->show();
                displayImage(filePath);
                // Pas de player pour les images
                controls->setImageMode(true);
                break;
        }
        
        // Reset des contrôles (sauf pour les images)
        if (mediaType != MediaType::IMAGE) {
            controls->setImageMode(false);
            controls->setProgressValue(0);
            controls->setTimeLabel("00:00", "00:00");
            controls->setPlayPauseIcon(false);
        }
        isPlaying = false;
        
        // Définir le volume (Qt5)
        player->setVolume(50);
    }
    
    void MediaPlayer::displayImage(const QString& filePath) {
        QPixmap pixmap(filePath);
        if (!pixmap.isNull()) {
            // Redimensionner l'image pour qu'elle s'adapte à la zone d'affichage
            QPixmap scaledPixmap = pixmap.scaled(
                800, 600,
                Qt::KeepAspectRatio,
                Qt::SmoothTransformation
            );
            mediaDisplay->setPixmap(scaledPixmap);
            mediaDisplay->setScaledContents(false);
        } else {
            mediaDisplay->setText("❌ Impossible de charger l'image");
        }
    }
    
    void MediaPlayer::displayAudioInfo(const QString& filePath) {
        QFileInfo fileInfo(filePath);
        QString audioInfo = QString("🎵 Audio chargé\\n\\n📀 %1\\n\\n🎼 Format: %2\\n\\n▶️ Cliquez sur lecture pour écouter")
                           .arg(fileInfo.baseName())
                           .arg(fileInfo.suffix().toUpper());
        mediaDisplay->setText(audioInfo);
    }
    
    void MediaPlayer::playMedia() {
        if (currentMediaPath.isEmpty()) return;
        if (mediaType == MediaType::IMAGE) return; // Pas de lecture pour les images
        
        player->play();
        isPlaying = true;
        controls->setPlayPauseIcon(true);
    }
    
    void MediaPlayer::pauseMedia() {
        player->pause();
        isPlaying = false;
        controls->setPlayPauseIcon(false);
    }
    
    void MediaPlayer::stopMedia() {
        player->stop();
        isPlaying = false;
        controls->setPlayPauseIcon(false);
        controls->setProgressValue(0);
        controls->setTimeLabel("00:00", "00:00");
    }
    
    void MediaPlayer::setVolume(int volume) {
        controls->setVolumeValue(volume);
        player->setVolume(volume);
    }
    
    void MediaPlayer::rewind5Seconds() {
        if (mediaType == MediaType::IMAGE) return;
        qint64 currentPos = player->position();
        qint64 newPos = qMax(qint64(0), currentPos - 5000); // Reculer de 5000ms (5s)
        player->setPosition(newPos);
    }
    
    void MediaPlayer::fastForward5Seconds() {
        if (mediaType == MediaType::IMAGE) return;
        qint64 currentPos = player->position();
        qint64 duration = player->duration();
        qint64 newPos = qMin(duration, currentPos + 5000); // Avancer de 5000ms (5s)
        player->setPosition(newPos);
    }
    
    void MediaPlayer::toggleFullscreen() {
        if (fullscreenCallback && !currentMediaPath.isEmpty()) {
            fullscreenCallback(currentMediaPath, mediaType, player);
        }
    }
    
    void MediaPlayer::setOnFullscreenRequested(std::function<void(const QString&, MediaType, QMediaPlayer*)> callback) {
        fullscreenCallback = callback;
    }
    
    void MediaPlayer::restorePlayerState(qint64 position, int volume, bool shouldPlay) {
        if (currentMediaPath.isEmpty()) return;
        
        // Pour les images, pas de restauration nécessaire
        if (mediaType == MediaType::IMAGE) return;
        
        // Si le média n'est pas chargé, attendre qu'il le soit
        if (player->duration() <= 0) {
            // Utiliser QTimer::singleShot pour éviter les problèmes de connexion
            QTimer::singleShot(100, this, [this, position, volume, shouldPlay]() {
                if (player->duration() > 0) {
                    // Définir la position
                    if (position > 0) {
                        player->setPosition(position);
                    }
                    
                    // Définir le volume
                    player->setVolume(volume);
                    controls->setVolumeValue(volume);
                    
                    // Reprendre la lecture si nécessaire
                    if (shouldPlay) {
                        player->play();
                        isPlaying = true;
                        controls->setPlayPauseIcon(true);
                    }
                } else {
                    // Réessayer si pas encore chargé
                    restorePlayerState(position, volume, shouldPlay);
                }
            });
            return;
        }
        
        // Média déjà chargé, restaurer immédiatement
        if (position > 0 && player->duration() > 0) {
            player->setPosition(position);
        }
        
        // Définir le volume
        player->setVolume(volume);
        controls->setVolumeValue(volume);
        
        // Reprendre la lecture si nécessaire
        if (shouldPlay) {
            player->play();
            isPlaying = true;
            controls->setPlayPauseIcon(true);
        } else {
            player->pause();
            isPlaying = false;
            controls->setPlayPauseIcon(false);
        }
    }
    
    void MediaPlayer::onPlayPause() {
        if (currentMediaPath.isEmpty()) return;
        if (mediaType == MediaType::IMAGE) return;
        
        if (isPlaying) {
            pauseMedia();
        } else {
            playMedia();
        }
    }
    
    void MediaPlayer::onStop() {
        stopMedia();
    }
    
    void MediaPlayer::onVolumeChanged(int value) {
        player->setVolume(value);
    }
    
    void MediaPlayer::updateProgress() {
        // Cette méthode n'est plus utilisée avec QMediaPlayer
        // La progression est mise à jour via onPositionChanged
    }
    
    void MediaPlayer::onPositionChanged(qint64 position) {
        if (player->duration() > 0) {
            int progressValue = (position * 100) / player->duration();
            controls->setProgressValue(progressValue);
            controls->setTimeLabel(formatTime(position), formatTime(player->duration()));
        }
    }
    
    void MediaPlayer::onDurationChanged(qint64 duration) {
        controls->setProgressRange(0, 100);
        if (duration > 0) {
            controls->setTimeLabel("00:00", formatTime(duration));
        }
    }
    
    void MediaPlayer::onMediaStatusChanged(QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::EndOfMedia) {
            stopMedia();
        }
    }
    
    void MediaPlayer::onProgressSliderMoved(int position) {
        if (player->duration() > 0) {
            qint64 newPosition = (position * player->duration()) / 100;
            player->setPosition(newPosition);
        }
    }
    
    QString MediaPlayer::formatTime(qint64 milliseconds) {
        qint64 seconds = milliseconds / 1000;
        qint64 minutes = seconds / 60;
        seconds = seconds % 60;
        return QString("%1:%2")
            .arg(minutes, 2, 10, QChar('0'))
            .arg(seconds, 2, 10, QChar('0'));
    }
}