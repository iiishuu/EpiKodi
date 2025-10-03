/*
** EPITECH PROJECT, 2025
** EpiKodi [WSL: Ubuntu]
** File description:
** media_list - Composant liste des médias (bande à gauche)
*/

#ifndef MEDIA_LIST_HPP
#define MEDIA_LIST_HPP

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include <QFileInfo>
#include <functional>
#include "components/file_explorer.hpp"

namespace epikodi {
    
    class MediaList : public QWidget {
        // Q_OBJECT temporairement commenté
        
    public:
        explicit MediaList(MediaType type, QWidget* parent = nullptr);
        virtual ~MediaList() = default;
        
        void addMedia(const QString& filePath);
        void addMediaList(const QStringList& filePaths);
        void clearMedia();
        void setOnMediaSelected(std::function<void(const QString&)> callback);
        
        // Méthodes publiques (anciennement slots)
        void onMediaItemClicked(QListWidgetItem* item);
        void onRemoveSelected();
        void onPlaySelected();
        
    private:
        void setupUI();
        void setupControls();
        QString getListStyle();
        QString getButtonStyle();
        QString getMediaIcon(const QString& filePath) const;
        
        MediaType mediaType;
        QVBoxLayout* mainLayout;
        QHBoxLayout* controlsLayout;
        
        QLabel* titleLabel;
        QListWidget* mediaListWidget;
        QPushButton* playButton;
        QPushButton* removeButton;
        QPushButton* clearAllButton;
        
        QStringList mediaPaths;
        std::function<void(const QString&)> mediaSelectedCallback;
    };
}

#endif // MEDIA_LIST_HPP