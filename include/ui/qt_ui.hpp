/*
** EPITECH PROJECT, 2025
** EpiKodi [WSL: Ubuntu]
** File description:
** qt_ui
*/

#ifndef QT_UI_HPP
#define QT_UI_HPP

#include "ui/ui_manager.hpp"      // ton interface
#include "media/media_file.hpp"   // MediaFile
#include <QApplication>           // QApplication
#include <QMainWindow>            // QMainWindow
#include <QListWidget>            // QListWidget
#include <QListWidgetItem>        // QListWidgetItem
#include <QMessageBox>            // QMessageBox
#include <QStatusBar>             // QStatusBar
#include <QVBoxLayout>            // QVBoxLayout
#include <QLabel>                 // QLabel
#include <QEvent>                 // QEvent
#include <QtMultimedia/QMediaPlayer>     // QMediaPlayer
#include <QtMultimediaWidgets/QVideoWidget> // QVideoWidget
#include <string>
#include <vector>

namespace epikodi {
    class QtUI : public UIManager, public QObject {
    public:
        QtUI(int &argc, char **argv);
        ~QtUI();

        void openWindow(const std::string &title) override;
        void closeWindow() override;
        void showMenu(const std::vector<std::string> &mediaFiles) override;

    protected:
        bool eventFilter(QObject *obj, QEvent *event) override;

    private:
        QApplication app;
        QMainWindow *window;
        QListWidget *listWidget;
        QMediaPlayer *player;
        QVideoWidget *videoWidget;
        QLabel *infoLabel;
        QLabel *imageLabel;      // Pour afficher les images
        QWidget *rightPanel;     // Panel de droite pour pouvoir switcher
        
        void updateFileInfo(const QString &fileName);
        void playSelectedFile(const QString &fileName);
        void displayImage(const QString &fileName);
        void displayVideo(const QString &fileName);
        void showVideoControls();
        void hideVideoControls();
    };
}

#endif // QT_UI_HPP
