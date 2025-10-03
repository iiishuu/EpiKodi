/*
** EPITECH PROJECT, 2025
** EpiKodi [WSL: Ubuntu]
** File description:
** video_page - Implémentation de la page vidéo
*/

#include "ui/video_page.hpp"

namespace epikodi {
    
    VideoPage::VideoPage(QWidget* parent) 
        : QWidget(parent) {
        setupUI();
        setupComponents();
    }
    
    void VideoPage::setupUI() {
        layout = new QVBoxLayout(this);
        layout->setAlignment(Qt::AlignTop);
        layout->setSpacing(15);
        layout->setContentsMargins(20, 80, 20, 20);
        
        // Bouton retour en position absolue
        backButton = new QPushButton("← Retour", this);
        backButton->setStyleSheet(getBackButtonStyle());
        backButton->setFixedSize(120, 40);
        backButton->move(20, 20);
        
        connect(backButton, &QPushButton::clicked, [this]() {
            if (mediaPlayer) {
                mediaPlayer->stopMedia();
            }
            if (backCallback) backCallback();
        });
        
        setStyleSheet("background: transparent;");
    }
    
    void VideoPage::setupComponents() {
        // Titre de la page
        titleLabel = new QLabel("🎬 Gestion Vidéo");
        titleLabel->setAlignment(Qt::AlignCenter);
        titleLabel->setStyleSheet(
            "font-size: 28px; "
            "color: white; "
            "font-weight: 400; "
            "margin-bottom: 15px; "
            "border: none; "
            "background: transparent;"
        );
        layout->addWidget(titleLabel);
        
        // Explorateur de fichiers
        fileExplorer = new FileExplorer(MediaType::VIDEO, this);
        fileExplorer->setMaximumHeight(250);
        layout->addWidget(fileExplorer);
        
        // Layout horizontal pour MediaList + MediaPlayer
        mediaLayout = new QHBoxLayout();
        mediaLayout->setSpacing(15);
        
        // Liste des médias (30% largeur)
        mediaList = new MediaList(MediaType::VIDEO, this);
        mediaList->setFixedWidth(280);
        mediaLayout->addWidget(mediaList);
        
        // Lecteur de médias (70% largeur)
        mediaPlayer = new MediaPlayer(MediaType::VIDEO, this);
        mediaLayout->addWidget(mediaPlayer, 1);
        
        layout->addLayout(mediaLayout, 1);
        
        // Connexions entre les composants
        fileExplorer->setOnFilesAdded([this](const QStringList& files) {
            mediaList->addMediaList(files);
        });
        
        mediaList->setOnMediaSelected([this](const QString& file) {
            mediaPlayer->loadMedia(file);
        });
    }
    
    void VideoPage::setOnFullscreenRequested(std::function<void(const QString&, MediaType, QMediaPlayer*)> callback) {
        fullscreenCallback = callback;
        if (mediaPlayer) {
            mediaPlayer->setOnFullscreenRequested(callback);
        }
    }
    
    QString VideoPage::getBackButtonStyle() {
        return "QPushButton { "
               "  background: rgba(59, 130, 246, 180); "
               "  border: 1px solid rgba(79, 150, 255, 120); "
               "  border-radius: 8px; "
               "  color: white; "
               "  font-size: 14px; "
               "  font-weight: 500; "
               "  padding: 10px 15px; "
               "} "
               "QPushButton:hover { "
               "  background: rgba(79, 150, 255, 200); "
               "} "
               "QPushButton:pressed { "
               "  background: rgba(37, 99, 235, 180); "
               "}";
    }
    
    void VideoPage::setOnBackRequested(std::function<void()> callback) {
        backCallback = callback;
    }
}