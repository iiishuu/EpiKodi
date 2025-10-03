/*
** EPITECH PROJECT, 2025
** EpiKodi [WSL: Ubuntu]
** File description:
** media_list - Implémentation du composant liste des médias
*/

#include "components/media_list.hpp"
#include <QFileInfo>

namespace epikodi {
    
    MediaList::MediaList(MediaType type, QWidget* parent)
        : QWidget(parent), mediaType(type) {
        setupUI();
        setupControls();
    }
    
    void MediaList::setupUI() {
        mainLayout = new QVBoxLayout(this);
        mainLayout->setSpacing(15);
        mainLayout->setContentsMargins(15, 15, 15, 15);
        
        // Titre
        QString typeText;
        switch (mediaType) {
            case MediaType::VIDEO:
                typeText = "🎬 Playlist Vidéo";
                break;
            case MediaType::AUDIO:
                typeText = "🎵 Playlist Audio";
                break;
            case MediaType::IMAGE:
                typeText = "🖼️ Galerie Images";
                break;
        }
        
        titleLabel = new QLabel(typeText);
        titleLabel->setStyleSheet(
            "font-size: 16px; "
            "color: white; "
            "font-weight: 600; "
            "margin-bottom: 10px; "
            "border: none; "
            "background: transparent;"
        );
        mainLayout->addWidget(titleLabel);
        
        // Liste des médias
        mediaListWidget = new QListWidget();
        mediaListWidget->setStyleSheet(getListStyle());
        mediaListWidget->setAlternatingRowColors(true);
        mediaListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        
        connect(mediaListWidget, &QListWidget::itemClicked, [this](QListWidgetItem* item) {
            onMediaItemClicked(item);
        });
        connect(mediaListWidget, &QListWidget::itemDoubleClicked, [this](QListWidgetItem* item) {
            onMediaItemClicked(item);
            onPlaySelected();
        });
        
        mainLayout->addWidget(mediaListWidget, 1);
        
        setStyleSheet(
            "background: rgba(20, 25, 35, 220); "
            "border: 1px solid rgba(50, 60, 80, 150); "
            "border-radius: 10px;"
        );
    }
    
    void MediaList::setupControls() {
        controlsLayout = new QHBoxLayout();
        controlsLayout->setSpacing(8);
        
        playButton = new QPushButton("▶️");
        removeButton = new QPushButton("🗑️");
        clearAllButton = new QPushButton("Clear");
        
        playButton->setToolTip("Lire le média sélectionné");
        removeButton->setToolTip("Supprimer le média sélectionné");
        clearAllButton->setToolTip("Vider la liste");
        
        QString buttonStyle = getButtonStyle();
        playButton->setStyleSheet(buttonStyle + 
            "QPushButton { background: rgba(34, 197, 94, 180); }"
            "QPushButton:hover { background: rgba(74, 222, 128, 200); }");
        removeButton->setStyleSheet(buttonStyle + 
            "QPushButton { background: rgba(239, 68, 68, 180); }"
            "QPushButton:hover { background: rgba(248, 113, 113, 200); }");
        clearAllButton->setStyleSheet(buttonStyle);
        
        playButton->setFixedSize(40, 32);
        removeButton->setFixedSize(40, 32);
        clearAllButton->setFixedSize(60, 32);
        
        connect(playButton, &QPushButton::clicked, [this]() { onPlaySelected(); });
        connect(removeButton, &QPushButton::clicked, [this]() { onRemoveSelected(); });
        connect(clearAllButton, &QPushButton::clicked, [this]() { clearMedia(); });
        
        controlsLayout->addWidget(playButton);
        controlsLayout->addWidget(removeButton);
        controlsLayout->addStretch();
        controlsLayout->addWidget(clearAllButton);
        
        mainLayout->addLayout(controlsLayout);
    }
    
    QString MediaList::getListStyle() {
        return "QListWidget { "
               "  background: rgba(10, 15, 25, 180); "
               "  border: 1px solid rgba(30, 40, 60, 120); "
               "  border-radius: 6px; "
               "  color: white; "
               "  font-size: 12px; "
               "  padding: 5px; "
               "} "
               "QListWidget::item { "
               "  padding: 6px 8px; "
               "  border-radius: 3px; "
               "  margin: 1px; "
               "} "
               "QListWidget::item:selected { "
               "  background: rgba(59, 130, 246, 180); "
               "  font-weight: 500; "
               "} "
               "QListWidget::item:hover { "
               "  background: rgba(79, 150, 255, 120); "
               "}";
    }
    
    QString MediaList::getButtonStyle() {
        return "QPushButton { "
               "  background: rgba(75, 85, 99, 180); "
               "  border: 1px solid rgba(107, 114, 128, 120); "
               "  border-radius: 4px; "
               "  color: white; "
               "  font-size: 11px; "
               "  font-weight: 500; "
               "} "
               "QPushButton:hover { "
               "  background: rgba(107, 114, 128, 200); "
               "} "
               "QPushButton:pressed { "
               "  background: rgba(55, 65, 81, 180); "
               "}";
    }
    
    QString MediaList::getMediaIcon(const QString& filePath) const {
        QFileInfo fileInfo(filePath);
        QString extension = fileInfo.suffix().toLower();
        
        switch (mediaType) {
            case MediaType::VIDEO:
                return "🎬";
            case MediaType::AUDIO:
                if (extension == "mp3") return "🎵";
                if (extension == "wav" || extension == "flac") return "🎼";
                return "🎵";
            case MediaType::IMAGE:
                if (extension == "gif") return "🎞️";
                return "🖼️";
            default:
                return "📄";
        }
    }
    
    void MediaList::addMedia(const QString& filePath) {
        if (!mediaPaths.contains(filePath)) {
            mediaPaths.append(filePath);
            QFileInfo fileInfo(filePath);
            QString icon = getMediaIcon(filePath);
            QString displayText = icon + " " + fileInfo.baseName();
            mediaListWidget->addItem(displayText);
        }
    }
    
    void MediaList::addMediaList(const QStringList& filePaths) {
        for (const QString& filePath : filePaths) {
            addMedia(filePath);
        }
    }
    
    void MediaList::clearMedia() {
        mediaListWidget->clear();
        mediaPaths.clear();
    }
    
    void MediaList::setOnMediaSelected(std::function<void(const QString&)> callback) {
        mediaSelectedCallback = callback;
    }
    
    void MediaList::onMediaItemClicked(QListWidgetItem* item) {
        if (!item) return;
        
        int index = mediaListWidget->row(item);
        if (index >= 0 && index < mediaPaths.size()) {
            QString filePath = mediaPaths[index];
            if (mediaSelectedCallback) {
                mediaSelectedCallback(filePath);
            }
        }
    }
    
    void MediaList::onRemoveSelected() {
        int currentRow = mediaListWidget->currentRow();
        if (currentRow >= 0 && currentRow < mediaPaths.size()) {
            mediaPaths.removeAt(currentRow);
            delete mediaListWidget->takeItem(currentRow);
        }
    }
    
    void MediaList::onPlaySelected() {
        int currentRow = mediaListWidget->currentRow();
        if (currentRow >= 0 && currentRow < mediaPaths.size()) {
            QString filePath = mediaPaths[currentRow];
            if (mediaSelectedCallback) {
                mediaSelectedCallback(filePath);
            }
        }
    }
}