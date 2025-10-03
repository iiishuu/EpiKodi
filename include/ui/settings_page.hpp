/*
** EPITECH PROJECT, 2025
** EpiKodi [WSL: Ubuntu]
** File description:
** settings_page - Page des paramètres
*/

#ifndef SETTINGS_PAGE_HPP
#define SETTINGS_PAGE_HPP

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <functional>

namespace epikodi {
    
    class SettingsPage : public QWidget {
    public:
        explicit SettingsPage(QWidget* parent = nullptr);
        ~SettingsPage() = default;
        
        void setOnBackRequested(std::function<void()> callback);
        
    private:
        void setupUI();
        QString getBackButtonStyle();
        
        QVBoxLayout* layout;
        QLabel* titleLabel;
        QLabel* contentLabel;
        QPushButton* backButton;
        
        std::function<void()> backCallback;
    };
}

#endif // SETTINGS_PAGE_HPP