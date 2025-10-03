/*
** EPITECH PROJECT, 2025
** EpiKodi [WSL: Ubuntu]
** File description:
** menu_page - Implémentation du menu principal
*/

#include "ui/menu_page.hpp"

namespace epikodi {
    
    MenuPage::MenuPage(QWidget* parent) 
        : QWidget(parent) {
        setupUI();
        setupTitle();
        setupButtons();
    }
    
    void MenuPage::setupUI() {
        mainLayout = new QVBoxLayout(this);
        mainLayout->setSpacing(40);
        mainLayout->setContentsMargins(40, 40, 40, 40);
        mainLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
        
        setStyleSheet(
            "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
            "    stop:0 rgba(30, 35, 45, 255), stop:1 rgba(15, 20, 30, 255)); "
            "border: none;"
        );
    }
    
    void MenuPage::setupTitle() {
        titleLabel = new QLabel("🎬 EpiKodi");
        titleLabel->setAlignment(Qt::AlignCenter);
        titleLabel->setStyleSheet(
            "font-size: 42px; "
            "color: white; "
            "font-weight: 300; "
            "margin-bottom: 0px; "
            "letter-spacing: 2px; "
            "border: none; "
            "background: transparent;"
        );
        mainLayout->addWidget(titleLabel);
    }
    
    void MenuPage::setupButtons() {
        // Ajout d'un stretch pour centrer verticalement les boutons
        mainLayout->addStretch(1);
        
        buttonsWidget = new QWidget();
        buttonsWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        buttonsWidget->setStyleSheet("border: none; background: transparent;");
        
        buttonsLayout = new QGridLayout(buttonsWidget);
        buttonsLayout->setSpacing(25);
        buttonsLayout->setAlignment(Qt::AlignCenter);
        
        // Création des 4 boutons
        videoButton = new QPushButton("🎬\nVidéo");
        imageButton = new QPushButton("🖼️\nImage");
        audioButton = new QPushButton("🎵\nAudio");
        settingsButton = new QPushButton("⚙️\nParamètres");
        
        // Application du style
        QString style = getButtonStyle();
        videoButton->setStyleSheet(style);
        imageButton->setStyleSheet(style);
        audioButton->setStyleSheet(style);
        settingsButton->setStyleSheet(style);
        
        // Taille des boutons plus grande
        QSize buttonSize(300, 220);
        videoButton->setFixedSize(buttonSize);
        imageButton->setFixedSize(buttonSize);
        audioButton->setFixedSize(buttonSize);
        settingsButton->setFixedSize(buttonSize);
        
        // Placement en grille 2x2
        buttonsLayout->addWidget(videoButton, 0, 0);
        buttonsLayout->addWidget(imageButton, 0, 1);
        buttonsLayout->addWidget(audioButton, 1, 0);
        buttonsLayout->addWidget(settingsButton, 1, 1);
        
        // Connexions
        connect(videoButton, &QPushButton::clicked, [this]() {
            if (videoCallback) videoCallback();
        });
        connect(imageButton, &QPushButton::clicked, [this]() {
            if (imageCallback) imageCallback();
        });
        connect(audioButton, &QPushButton::clicked, [this]() {
            if (audioCallback) audioCallback();
        });
        connect(settingsButton, &QPushButton::clicked, [this]() {
            if (settingsCallback) settingsCallback();
        });
        
        mainLayout->addWidget(buttonsWidget, 0, Qt::AlignCenter);
        mainLayout->addStretch(1);
    }
    
    QString MenuPage::getButtonStyle() {
        return "QPushButton { "
               "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
               "    stop:0 rgba(55, 65, 85, 240), stop:0.5 rgba(35, 45, 65, 250), "
               "    stop:1 rgba(25, 35, 55, 255)); "
               "  border: 2px solid rgba(80, 100, 130, 200); "
               "  border-radius: 20px; "
               "  color: rgba(255, 255, 255, 250); "
               "  font-size: 24px; "
               "  font-weight: 600; "
               "  text-align: center; "
               "} "
               "QPushButton:hover { "
               "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
               "    stop:0 rgba(45, 75, 135, 255), stop:1 rgba(25, 45, 95, 255)); "
               "  border: 2px solid rgba(70, 140, 255, 255); "
               "  color: #f0f8ff; "
               "  font-size: 26px; "
               "} "
               "QPushButton:pressed { "
               "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
               "    stop:0 rgba(30, 40, 60, 230), stop:1 rgba(20, 30, 50, 240)); "
               "}";
    }
    
    void MenuPage::setOnVideoRequested(std::function<void()> callback) {
        videoCallback = callback;
    }
    
    void MenuPage::setOnImageRequested(std::function<void()> callback) {
        imageCallback = callback;
    }
    
    void MenuPage::setOnAudioRequested(std::function<void()> callback) {
        audioCallback = callback;
    }
    
    void MenuPage::setOnSettingsRequested(std::function<void()> callback) {
        settingsCallback = callback;
    }
}