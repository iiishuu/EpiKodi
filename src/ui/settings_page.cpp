/*
** EPITECH PROJECT, 2025
** EpiKodi [WSL: Ubuntu]
** File description:
** settings_page - Implémentation de la page paramètres
*/

#include "ui/settings_page.hpp"

namespace epikodi {
    
    SettingsPage::SettingsPage(QWidget* parent) 
        : QWidget(parent) {
        setupUI();
    }
    
    void SettingsPage::setupUI() {
        layout = new QVBoxLayout(this);
        layout->setAlignment(Qt::AlignCenter);
        layout->setSpacing(30);
        layout->setContentsMargins(20, 80, 20, 20);
        
        // Bouton retour en position absolue
        backButton = new QPushButton("← Retour", this);
        backButton->setStyleSheet(getBackButtonStyle());
        backButton->setFixedSize(120, 40);
        backButton->move(20, 20);
        
        connect(backButton, &QPushButton::clicked, [this]() {
            if (backCallback) backCallback();
        });
        
        layout->addStretch();
        
        // Titre
        titleLabel = new QLabel("⚙️ Paramètres");
        titleLabel->setAlignment(Qt::AlignCenter);
        titleLabel->setStyleSheet(
            "font-size: 36px; "
            "color: white; "
            "font-weight: 300; "
            "margin-bottom: 20px;"
        );
        layout->addWidget(titleLabel);
        
        // Contenu
        contentLabel = new QLabel("Configuration de l'application\n\n• Préférences d'affichage\n• Paramètres audio/vidéo\n• Raccourcis clavier\n• Thèmes et apparence\n• Gestion des dossiers\n\n(En développement)");
        contentLabel->setAlignment(Qt::AlignCenter);
        contentLabel->setStyleSheet(
            "font-size: 16px; "
            "color: rgba(255, 255, 255, 180); "
            "line-height: 1.8; "
            "background: rgba(30, 40, 60, 100); "
            "border: 1px solid rgba(60, 80, 110, 100); "
            "border-radius: 10px; "
            "padding: 30px;"
        );
        layout->addWidget(contentLabel);
        
        layout->addStretch();
        
        setStyleSheet("background: transparent;");
    }
    
    QString SettingsPage::getBackButtonStyle() {
        return "QPushButton { "
               "  background: rgba(100, 116, 139, 180); "
               "  border: 1px solid rgba(148, 163, 184, 120); "
               "  border-radius: 8px; "
               "  color: white; "
               "  font-size: 14px; "
               "  font-weight: 500; "
               "  padding: 10px 15px; "
               "} "
               "QPushButton:hover { "
               "  background: rgba(148, 163, 184, 200); "
               "} "
               "QPushButton:pressed { "
               "  background: rgba(71, 85, 105, 180); "
               "}";
    }
    
    void SettingsPage::setOnBackRequested(std::function<void()> callback) {
        backCallback = callback;
    }
}