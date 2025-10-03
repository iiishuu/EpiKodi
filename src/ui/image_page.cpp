/*
** EPITECH PROJECT, 2025
** EpiKodi [WSL: Ubuntu]
** File descripti        // Connexions entre les composants
        fileExplorer->setOnFilesAdded([this](const QStringList& files) {
            mediaList->addMediaList(files);
        });
        
        mediaList->setOnMediaSelected([this](const QString& file) {
            mediaPlayer->loadMedia(file);
        });_page - Implémentation de la page image
*/

#include "ui/image_page.hpp"

namespace epikodi {
    
    ImagePage::ImagePage(QWidget* parent) 
        : QWidget(parent) {
        setupUI();
        setupComponents();
    }
    
    void ImagePage::setupUI() {
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
    
    void ImagePage::setupComponents() {
        // Titre
        titleLabel = new QLabel("🖼️ Galerie d'Images");
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
        fileExplorer = new FileExplorer(MediaType::IMAGE, this);
        fileExplorer->setMaximumHeight(250);
        layout->addWidget(fileExplorer);
        
        // Layout horizontal
        mediaLayout = new QHBoxLayout();
        mediaLayout->setSpacing(15);
        
        // Liste des médias
        mediaList = new MediaList(MediaType::IMAGE, this);
        mediaList->setFixedWidth(280);
        mediaLayout->addWidget(mediaList);
        
        // Lecteur de médias
        mediaPlayer = new MediaPlayer(MediaType::IMAGE, this);
        mediaLayout->addWidget(mediaPlayer, 1);
        
        layout->addLayout(mediaLayout, 1);
        
        // Connexions
        fileExplorer->setOnFilesAdded([this](const QStringList& files) {
            mediaList->addMediaList(files);
        });
        
        mediaList->setOnMediaSelected([this](const QString& file) {
            mediaPlayer->loadMedia(file);
        });
    }
    
    void ImagePage::setOnFullscreenRequested(std::function<void(const QString&, MediaType, QMediaPlayer*)> callback) {
        fullscreenCallback = callback;
        if (mediaPlayer) {
            mediaPlayer->setOnFullscreenRequested(callback);
        }
    }
    
    QString ImagePage::getBackButtonStyle() {
        return "QPushButton { "
               "  background: rgba(34, 197, 94, 180); "
               "  border: 1px solid rgba(74, 222, 128, 120); "
               "  border-radius: 8px; "
               "  color: white; "
               "  font-size: 14px; "
               "  font-weight: 500; "
               "  padding: 10px 15px; "
               "} "
               "QPushButton:hover { "
               "  background: rgba(74, 222, 128, 200); "
               "} "
               "QPushButton:pressed { "
               "  background: rgba(21, 128, 61, 180); "
               "}";
    }
    
    void ImagePage::setOnBackRequested(std::function<void()> callback) {
        backCallback = callback;
    }
}