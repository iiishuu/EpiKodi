/*
** EPITECH PROJECT, 2025
** EpiKodi [WSL: Ubuntu]
** File description:
** menu_page - Page du menu principal avec 4 boutons
*/

#ifndef MENU_PAGE_HPP
#define MENU_PAGE_HPP

#include <QWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <functional>

namespace epikodi {
    
    class MenuPage : public QWidget {
    public:
        explicit MenuPage(QWidget* parent = nullptr);
        ~MenuPage() = default;
        
        void setOnVideoRequested(std::function<void()> callback);
        void setOnImageRequested(std::function<void()> callback);
        void setOnAudioRequested(std::function<void()> callback);
        void setOnSettingsRequested(std::function<void()> callback);
        
    private:
        void setupUI();
        void setupTitle();
        void setupButtons();
        QString getButtonStyle();
        
        QVBoxLayout* mainLayout;
        QLabel* titleLabel;
        QGridLayout* buttonsLayout;
        QWidget* buttonsWidget;
        
        QPushButton* videoButton;
        QPushButton* imageButton;
        QPushButton* audioButton;
        QPushButton* settingsButton;
        
        std::function<void()> videoCallback;
        std::function<void()> imageCallback;
        std::function<void()> audioCallback;
        std::function<void()> settingsCallback;
    };
}

#endif // MENU_PAGE_HPP