/*
** EPITECH PROJECT, 2025
** EpiKodi [WSL: Ubuntu]
** File description:
** video_page - Page dédiée aux vidéos
*/

#ifndef VIDEO_PAGE_HPP
#define VIDEO_PAGE_HPP

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <functional>
#include "components/file_explorer.hpp"
#include "components/media_list.hpp"
#include "components/media_player.hpp"

namespace epikodi {
    
    class VideoPage : public QWidget {
    public:
        explicit VideoPage(QWidget* parent = nullptr);
        ~VideoPage() = default;
        
        void setOnBackRequested(std::function<void()> callback);
        void setOnFullscreenRequested(std::function<void(const QString&, MediaType, QMediaPlayer*)> callback);
        
        MediaPlayer* getMediaPlayer() { return mediaPlayer; }
        
    private:
        void setupUI();
        void setupComponents();
        QString getBackButtonStyle();
        
        QVBoxLayout* layout;
        QHBoxLayout* mediaLayout;
        QLabel* titleLabel;
        QPushButton* backButton;
        
        // Composants
        FileExplorer* fileExplorer;
        MediaList* mediaList;
        MediaPlayer* mediaPlayer;
        
        std::function<void()> backCallback;
        std::function<void(const QString&, MediaType, QMediaPlayer*)> fullscreenCallback;
    };
}

#endif // VIDEO_PAGE_HPP