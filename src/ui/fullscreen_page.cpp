/*
** EPITECH PROJECT, 2025
** EpiKodi [WSL: Ubuntu]
** File description:
** fullscreen_page - Implémentation de la page plein écran
*/

#include "ui/fullscreen_page.hpp"
#include <QKeyEvent>
#include <QPixmap>
#include <QFileInfo>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

namespace epikodi {
    
    FullscreenPage::FullscreenPage(QWidget* parent)
        : QWidget(parent), isPlaying(false), controlsVisible(true) {
        
        setupUI();
        
        // Timer pour masquer automatiquement les contrôles après 3 secondes
        hideControlsTimer = new QTimer(this);
        hideControlsTimer->setInterval(3000);
        hideControlsTimer->setSingleShot(true);
        connect(hideControlsTimer, &QTimer::timeout, [this]() {
            onControlsTimeout();
        });
        
        // Activer le suivi de la souris
        setMouseTracking(true);
        
        // Masquer les contrôles initialement
        controlsContainer->setGeometry(0, height() - 100, width(), 100);
        
        // Démarrer le timer
        hideControlsTimer->start();
    }
    
    void FullscreenPage::setupUI() {
        mainLayout = new QVBoxLayout(this);
        mainLayout->setSpacing(0);
        mainLayout->setContentsMargins(0, 0, 0, 0);
        
        // Container principal avec fond noir
        setStyleSheet("background: black;");
        
        // Container pour l'affichage du média
        displayContainer = new QWidget();
        displayContainer->setStyleSheet("background: black;");
        QVBoxLayout* displayLayout = new QVBoxLayout(displayContainer);
        displayLayout->setContentsMargins(0, 0, 0, 0);
        
        // Label pour images et audio
        mediaDisplay = new QLabel();
        mediaDisplay->setAlignment(Qt::AlignCenter);
        mediaDisplay->setStyleSheet("background: black; color: white;");
        mediaDisplay->hide();
        
        // Widget vidéo
        videoWidget = new QVideoWidget();
        videoWidget->setStyleSheet("background: black;");
        videoWidget->hide();
        
        displayLayout->addWidget(mediaDisplay);
        displayLayout->addWidget(videoWidget);
        
        mainLayout->addWidget(displayContainer, 1);
        
        // Container pour les contrôles (en bas, position absolue pour animation)
        controlsContainer = new QWidget(this);
        controlsContainer->setStyleSheet(
            "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
            "stop:0 rgba(0, 0, 0, 0), stop:0.3 rgba(0, 0, 0, 150), stop:1 rgba(0, 0, 0, 200));"
        );
        controlsContainer->setFixedHeight(120);
        
        QVBoxLayout* controlsWrapperLayout = new QVBoxLayout(controlsContainer);
        controlsWrapperLayout->setContentsMargins(20, 20, 20, 20);
        
        controls = new MediaControls();
        controlsWrapperLayout->addWidget(controls);
        
        // Ne pas ajouter au layout principal - utiliser position absolue
        // mainLayout->addWidget(controlsContainer);
        
        // Initialiser le lecteur
        player = new QMediaPlayer(this);
        
        // Connexions des contrôles
        controls->setOnPlayPause([this]() {
            if (isPlaying) {
                player->pause();
                isPlaying = false;
                controls->setPlayPauseIcon(false);
            } else {
                player->play();
                isPlaying = true;
                controls->setPlayPauseIcon(true);
            }
        });
        
        controls->setOnStop([this]() {
            player->stop();
            isPlaying = false;
            controls->setPlayPauseIcon(false);
            if (backCallback) backCallback();
        });
        
        controls->setOnRewind([this]() {
            qint64 currentPos = player->position();
            qint64 newPos = qMax(qint64(0), currentPos - 5000);
            player->setPosition(newPos);
        });
        
        controls->setOnFastForward([this]() {
            qint64 currentPos = player->position();
            qint64 duration = player->duration();
            qint64 newPos = qMin(duration, currentPos + 5000);
            player->setPosition(newPos);
        });
        
        controls->setOnFullscreen([this]() {
            // Quitter le plein écran
            if (backCallback) backCallback();
        });
        
        controls->setOnProgressChanged([this](int position) {
            if (player->duration() > 0) {
                qint64 newPosition = (position * player->duration()) / 100;
                player->setPosition(newPosition);
            }
        });
        
        controls->setOnVolumeChanged([this](int value) {
            player->setVolume(value);
        });
        
        // Connexions du player
        connect(player, &QMediaPlayer::positionChanged, [this](qint64 position) {
            if (player->duration() > 0) {
                int progressValue = (position * 100) / player->duration();
                controls->setProgressValue(progressValue);
                
                auto formatTime = [](qint64 ms) -> QString {
                    qint64 seconds = ms / 1000;
                    qint64 minutes = seconds / 60;
                    seconds = seconds % 60;
                    return QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));
                };
                
                controls->setTimeLabel(formatTime(position), formatTime(player->duration()));
            }
        });
        
        connect(player, &QMediaPlayer::durationChanged, [this](qint64 duration) {
            controls->setProgressRange(0, 100);
        });
        
        connect(player, &QMediaPlayer::mediaStatusChanged, [this](QMediaPlayer::MediaStatus status) {
            if (status == QMediaPlayer::EndOfMedia) {
                isPlaying = false;
                controls->setPlayPauseIcon(false);
            }
        });
        
        // Icône plein écran pour "quitter"
        controls->setFullscreenIcon(true);
    }
    
    void FullscreenPage::loadMedia(const QString& filePath, MediaType type, QMediaPlayer* sourcePlayer) {
        if (filePath.isEmpty()) return;
        
        currentMediaPath = filePath;
        mediaType = type;
        
        // Copier l'état du player source
        qint64 currentPosition = 0;
        int volume = 50;
        bool wasPlaying = false;
        
        if (sourcePlayer) {
            currentPosition = sourcePlayer->position();
            volume = sourcePlayer->volume();
            wasPlaying = (sourcePlayer->state() == QMediaPlayer::PlayingState);
            sourcePlayer->pause(); // Mettre en pause le player source
        }
        
        // S'assurer que les contrôles sont visibles au démarrage
        controlsContainer->setGeometry(0, height() - 120, width(), 120);
        controlsVisible = true;
        setCursor(Qt::ArrowCursor);
        
        // Charger le média
        switch (mediaType) {
            case MediaType::VIDEO:
                videoWidget->show();
                mediaDisplay->hide();
                player->setVideoOutput(videoWidget);
                player->setMedia(QUrl::fromLocalFile(filePath));
                break;
                
            case MediaType::AUDIO: {
                videoWidget->hide();
                mediaDisplay->show();
                QFileInfo fileInfo(filePath);
                QString audioInfo = QString("🎵 Lecture Audio\n\n%1").arg(fileInfo.baseName());
                mediaDisplay->setText(audioInfo);
                mediaDisplay->setStyleSheet(
                    "font-size: 24px; "
                    "color: white; "
                    "background: black;"
                );
                player->setMedia(QUrl::fromLocalFile(filePath));
                break;
            }
                
            case MediaType::IMAGE: {
                videoWidget->hide();
                mediaDisplay->show();
                QPixmap pixmap(filePath);
                if (!pixmap.isNull()) {
                    QPixmap scaledPixmap = pixmap.scaled(
                        size(),
                        Qt::KeepAspectRatio,
                        Qt::SmoothTransformation
                    );
                    mediaDisplay->setPixmap(scaledPixmap);
                }
                // Mode image : n'afficher que le bouton plein écran
                controls->setImageMode(true);
                return;
            }
        }
        
        // Restaurer l'état
        player->setVolume(volume);
        controls->setVolumeValue(volume);
        
        // Remettre en mode normal pour vidéo/audio
        if (mediaType != MediaType::IMAGE) {
            controls->setImageMode(false);
        }
        
        if (currentPosition > 0) {
            player->setPosition(currentPosition);
        }
        
        if (wasPlaying) {
            player->play();
            isPlaying = true;
            controls->setPlayPauseIcon(true);
        }
    }
    
    void FullscreenPage::mouseMoveEvent(QMouseEvent* event) {
        QWidget::mouseMoveEvent(event);
        showControls();
        hideControlsTimer->start(); // Redémarrer le timer
    }
    
    void FullscreenPage::keyPressEvent(QKeyEvent* event) {
        if (event->key() == Qt::Key_Escape || event->key() == Qt::Key_F) {
            // ESC ou F pour quitter le plein écran
            if (backCallback) backCallback();
        } else if (event->key() == Qt::Key_Space) {
            // Espace pour play/pause
            if (isPlaying) {
                player->pause();
                isPlaying = false;
                controls->setPlayPauseIcon(false);
            } else {
                player->play();
                isPlaying = true;
                controls->setPlayPauseIcon(true);
            }
        } else if (event->key() == Qt::Key_Left) {
            // Flèche gauche : reculer 5s
            qint64 currentPos = player->position();
            qint64 newPos = qMax(qint64(0), currentPos - 5000);
            player->setPosition(newPos);
        } else if (event->key() == Qt::Key_Right) {
            // Flèche droite : avancer 5s
            qint64 currentPos = player->position();
            qint64 duration = player->duration();
            qint64 newPos = qMin(duration, currentPos + 5000);
            player->setPosition(newPos);
        }
        
        QWidget::keyPressEvent(event);
    }
    
    void FullscreenPage::resizeEvent(QResizeEvent* event) {
        QWidget::resizeEvent(event);
        
        // Repositionner les contrôles en fonction de la visibilité
        if (controlsVisible) {
            controlsContainer->setGeometry(0, height() - 120, width(), 120);
        } else {
            controlsContainer->setGeometry(0, height(), width(), 120);
        }
    }
    
    void FullscreenPage::showControls() {
        if (!controlsVisible) {
            // Animation slide up
            QPropertyAnimation* slideAnimation = new QPropertyAnimation(controlsContainer, "geometry");
            slideAnimation->setDuration(300);
            slideAnimation->setEasingCurve(QEasingCurve::OutCubic);
            slideAnimation->setStartValue(QRect(0, height(), width(), 120));
            slideAnimation->setEndValue(QRect(0, height() - 120, width(), 120));
            slideAnimation->start(QAbstractAnimation::DeleteWhenStopped);
            
            controlsVisible = true;
            setCursor(Qt::ArrowCursor);
        }
    }
    
    void FullscreenPage::hideControls() {
        if (controlsVisible && isPlaying) {
            // Animation slide down
            QPropertyAnimation* slideAnimation = new QPropertyAnimation(controlsContainer, "geometry");
            slideAnimation->setDuration(300);
            slideAnimation->setEasingCurve(QEasingCurve::InCubic);
            slideAnimation->setStartValue(QRect(0, height() - 120, width(), 120));
            slideAnimation->setEndValue(QRect(0, height(), width(), 120));
            slideAnimation->start(QAbstractAnimation::DeleteWhenStopped);
            
            controlsVisible = false;
            setCursor(Qt::BlankCursor);
        }
    }
    
    void FullscreenPage::onControlsTimeout() {
        hideControls();
    }
    
    void FullscreenPage::setOnBack(std::function<void()> callback) {
        backCallback = callback;
    }
    
    qint64 FullscreenPage::getCurrentPosition() const {
        return player ? player->position() : 0;
    }
    
    int FullscreenPage::getCurrentVolume() const {
        return player ? player->volume() : 50;
    }
    
    bool FullscreenPage::getIsPlaying() const {
        return isPlaying;
    }
    
    void FullscreenPage::stopPlayer() {
        if (player) {
            player->stop();
            isPlaying = false;
        }
    }
}
