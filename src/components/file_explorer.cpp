/*
** EPITECH PROJECT, 2025
** EpiKodi [WSL: Ubuntu]
** File description:
** file_explorer - Implémentation du composant explorateur de fichiers
*/

#include "components/file_explorer.hpp"
#include <QFileInfo>
#include <QMimeDatabase>
#include <QStandardPaths>

namespace epikodi {
    
    FileExplorer::FileExplorer(MediaType type, QWidget* parent)
        : QWidget(parent), mediaType(type) {
        setupUI();
        setupButtons();
    }
    
    void FileExplorer::setupUI() {
        mainLayout = new QVBoxLayout(this);
        mainLayout->setSpacing(15);
        mainLayout->setContentsMargins(20, 20, 20, 20);
        
        // Titre
        titleLabel = new QLabel("📁 " + getMediaTypeText());
        titleLabel->setStyleSheet(
            "font-size: 18px; "
            "color: white; "
            "font-weight: 600; "
            "margin-bottom: 10px; "
            "border: none; "
            "background: transparent;"
        );
        mainLayout->addWidget(titleLabel);
        
        setStyleSheet(
            "background: rgba(25, 30, 40, 200); "
            "border: 1px solid rgba(60, 70, 90, 150); "
            "border-radius: 12px;"
        );
    }
    
    void FileExplorer::setupButtons() {
        buttonsLayout = new QHBoxLayout();
        buttonsLayout->setSpacing(10);
        
        addFileButton = new QPushButton("+ Fichier");
        addFolderButton = new QPushButton("+ Dossier");
        
        QString buttonStyle = getButtonStyle();
        addFileButton->setStyleSheet(buttonStyle);
        addFolderButton->setStyleSheet(buttonStyle);
        
        connect(addFileButton, &QPushButton::clicked, [this]() { openFileDialog(); });
        connect(addFolderButton, &QPushButton::clicked, [this]() { openFolderDialog(); });
        
        buttonsLayout->addWidget(addFileButton, 1);
        buttonsLayout->addWidget(addFolderButton, 1);
        
        mainLayout->addLayout(buttonsLayout);
        mainLayout->addStretch();
    }
    
    QString FileExplorer::getButtonStyle() {
        return "QPushButton { "
               "  background: rgba(59, 130, 246, 180); "
               "  border: 1px solid rgba(79, 150, 255, 120); "
               "  border-radius: 6px; "
               "  color: white; "
               "  font-size: 13px; "
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
    
    QStringList FileExplorer::getFileFilters() const {
        switch (mediaType) {
            case MediaType::VIDEO:
                return {"*.mp4", "*.avi", "*.mkv", "*.mov", "*.wmv", "*.flv", "*.webm"};
            case MediaType::AUDIO:
                return {"*.mp3", "*.wav", "*.flac", "*.aac", "*.ogg", "*.m4a", "*.wma"};
            case MediaType::IMAGE:
                return {"*.jpg", "*.jpeg", "*.png", "*.gif", "*.bmp", "*.tiff", "*.webp"};
            default:
                return {};
        }
    }
    
    QString FileExplorer::getMediaTypeText() const {
        switch (mediaType) {
            case MediaType::VIDEO:
                return "Explorateur Vidéo";
            case MediaType::AUDIO:
                return "Explorateur Audio";
            case MediaType::IMAGE:
                return "Explorateur Images";
            default:
                return "Explorateur";
        }
    }
    
    bool FileExplorer::isValidFileType(const QString& filePath) const {
        QFileInfo fileInfo(filePath);
        QString extension = "*." + fileInfo.suffix().toLower();
        return getFileFilters().contains(extension);
    }
    
    void FileExplorer::openFileDialog() {
        QStringList filters = getFileFilters();
        QString filter = getMediaTypeText() + " (" + filters.join(" ") + ")";
        
        QStringList files = QFileDialog::getOpenFileNames(
            this,
            "Sélectionner des fichiers " + getMediaTypeText().toLower(),
            QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
            filter
        );
        
        if (!files.isEmpty() && filesAddedCallback) {
            filesAddedCallback(files);
        }
    }
    
    void FileExplorer::openFolderDialog() {
        QString dir = QFileDialog::getExistingDirectory(
            this,
            "Sélectionner un dossier",
            QStandardPaths::writableLocation(QStandardPaths::HomeLocation)
        );
        
        if (!dir.isEmpty()) {
            QDir directory(dir);
            QStringList filters = getFileFilters();
            QStringList files = directory.entryList(filters, QDir::Files);
            
            QStringList fullPaths;
            for (const QString& file : files) {
                QString fullPath = directory.absoluteFilePath(file);
                fullPaths.append(fullPath);
            }
            
            if (!fullPaths.isEmpty() && filesAddedCallback) {
                filesAddedCallback(fullPaths);
            }
        }
    }
    
    void FileExplorer::setOnFilesAdded(std::function<void(const QStringList&)> callback) {
        filesAddedCallback = callback;
    }
}