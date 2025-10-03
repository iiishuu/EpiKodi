/*
** EPITECH PROJECT, 2025
** EpiKodi [WSL: Ubuntu]
** File description:
** file_explorer - Composant explorateur de fichiers avec filtres par type de média
*/

#ifndef FILE_EXPLORER_HPP
#define FILE_EXPLORER_HPP

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QDir>
#include <QFileInfo>
#include <functional>

namespace epikodi {
    
    enum class MediaType {
        VIDEO,
        AUDIO,
        IMAGE
    };
    
    class FileExplorer : public QWidget {
        // Q_OBJECT temporairement commenté
        
    public:
        explicit FileExplorer(MediaType type, QWidget* parent = nullptr);
        virtual ~FileExplorer() = default;
        
        void setOnFilesAdded(std::function<void(const QStringList&)> callback);
        
        // Méthodes publiques (anciennement slots)
        void openFileDialog();
        void openFolderDialog();
        
    private:
        void setupUI();
        void setupButtons();
        QString getButtonStyle();
        QStringList getFileFilters() const;
        QString getMediaTypeText() const;
        bool isValidFileType(const QString& filePath) const;
        
        MediaType mediaType;
        QVBoxLayout* mainLayout;
        QHBoxLayout* buttonsLayout;
        
        QPushButton* addFileButton;
        QPushButton* addFolderButton;
        QLabel* titleLabel;
        
        std::function<void(const QStringList&)> filesAddedCallback;
    };
}

#endif // FILE_EXPLORER_HPP