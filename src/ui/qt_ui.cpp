/*
** EPITECH PROJECT, 2025
** EpiKodi [WSL: Ubuntu]
** File description:
** qt_ui
*/

#include "ui/qt_ui.hpp"
#include <QString>
#include <QDir>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QFileInfo>
#include <QDebug>
#include <QKeyEvent>
#include <QLabel>
#include <QSplitter>

namespace epikodi {

QtUI::QtUI(int &argc, char **argv)
    : app(argc, argv), window(nullptr), listWidget(nullptr),
      player(nullptr), videoWidget(nullptr), infoLabel(nullptr), 
      imageLabel(nullptr), rightPanel(nullptr) {}

QtUI::~QtUI() {
    delete player;
    delete window;
}

void QtUI::openWindow(const std::string &title) {
    window = new QMainWindow();
    window->setWindowTitle(QString::fromStdString(title));
    window->resize(1200, 800);

    QWidget *central = new QWidget(window);
    QHBoxLayout *mainLayout = new QHBoxLayout(central);
    
    // Splitter principal
    QSplitter *splitter = new QSplitter(Qt::Horizontal, central);
    
    // --- Zone de gauche : Liste des médias ---
    QWidget *leftPanel = new QWidget();
    leftPanel->setMaximumWidth(400);
    leftPanel->setMinimumWidth(300);
    
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    
    // Titre de la liste
    QLabel *listTitle = new QLabel("📁 Bibliothèque Multimédia");
    listTitle->setStyleSheet("font-size: 14px; font-weight: bold; padding: 8px;");
    leftLayout->addWidget(listTitle);
    
    // Liste des fichiers
    listWidget = new QListWidget();
    listWidget->setStyleSheet(
        "QListWidget {"
        "  background-color: #2b2b2b;"
        "  color: white;"
        "  font-size: 12px;"
        "  selection-background-color: #3d7eff;"
        "  border: 1px solid #555;"
        "}"
        "QListWidget::item {"
        "  padding: 8px;"
        "  border-bottom: 1px solid #444;"
        "}"
        "QListWidget::item:hover {"
        "  background-color: #404040;"
        "}"
    );
    leftLayout->addWidget(listWidget);
    
    // Zone d'info sur le fichier sélectionné
    infoLabel = new QLabel("Sélectionnez un fichier pour voir les détails");
    infoLabel->setWordWrap(true);
    infoLabel->setStyleSheet(
        "padding: 12px; "
        "background-color: #1e1e1e; "
        "color: #ccc; "
        "border: 1px solid #555; "
        "border-radius: 4px;"
    );
    infoLabel->setMaximumHeight(120);
    leftLayout->addWidget(infoLabel);
    
    splitter->addWidget(leftPanel);
    
    // --- Zone de droite : Lecture ---
    rightPanel = new QWidget();
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
    
    // Zone vidéo
    videoWidget = new QVideoWidget();
    videoWidget->setStyleSheet("background-color: black; border: 2px solid #555;");
    rightLayout->addWidget(videoWidget);
    
    // Zone image (cachée par défaut)
    imageLabel = new QLabel();
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setStyleSheet("background-color: black; border: 2px solid #555; color: white;");
    imageLabel->setText("Sélectionnez une image pour l'afficher");
    imageLabel->setMinimumSize(400, 300);
    imageLabel->setScaledContents(false); // On gère manuellement le redimensionnement
    imageLabel->hide(); // Caché par défaut
    rightLayout->addWidget(imageLabel);
    
    // Contrôles de lecture
    QWidget *controlsWidget = new QWidget();
    QHBoxLayout *controlsLayout = new QHBoxLayout(controlsWidget);
    
    QPushButton *playBtn = new QPushButton("▶️ Play");
    QPushButton *pauseBtn = new QPushButton("⏸️ Pause");
    QPushButton *stopBtn = new QPushButton("⏹️ Stop");
    
    // Style des boutons
    QString btnStyle = 
        "QPushButton { "
        "  padding: 8px 16px; "
        "  font-size: 12px; "
        "  background-color: #3d7eff; "
        "  color: white; "
        "  border: none; "
        "  border-radius: 4px; "
        "} "
        "QPushButton:hover { "
        "  background-color: #4a8fff; "
        "} "
        "QPushButton:pressed { "
        "  background-color: #2a6eef; "
        "}";
    
    playBtn->setStyleSheet(btnStyle);
    pauseBtn->setStyleSheet(btnStyle);
    stopBtn->setStyleSheet(btnStyle);
    
    controlsLayout->addWidget(playBtn);
    controlsLayout->addWidget(pauseBtn);
    controlsLayout->addWidget(stopBtn);
    controlsLayout->addStretch();
    
    rightLayout->addWidget(controlsWidget);
    splitter->addWidget(rightPanel);
    
    // Configuration du splitter
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);
    
    mainLayout->addWidget(splitter);

    // Player
    player = new QMediaPlayer(window);
    player->setVideoOutput(videoWidget);
    
    // Connexions des boutons
    QObject::connect(playBtn, &QPushButton::clicked, [this]() {
        if (player->state() != QMediaPlayer::PlayingState) {
            player->play();
        }
    });
    
    QObject::connect(pauseBtn, &QPushButton::clicked, [this]() {
        player->pause();
    });
    
    QObject::connect(stopBtn, &QPushButton::clicked, [this]() {
        player->stop();
    });
    
    // Gestion des erreurs de QMediaPlayer
    QObject::connect(player, QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error),
        [this](QMediaPlayer::Error error) {
            QString errorString = player->errorString();
            QMessageBox::critical(window, "Erreur Media Player", 
                "Erreur: " + errorString + "\n\n" +
                "Solutions possibles:\n" +
                "1. Vérifiez que les codecs sont installés\n" +
                "2. Testez avec un autre format vidéo\n" +
                "3. Utilisez une solution alternative (VLC, mpv)");
            qDebug() << "Media player error:" << error << errorString;
        });

    // Status du player
    QObject::connect(player, &QMediaPlayer::stateChanged,
        [this](QMediaPlayer::State state) {
            QString statusText;
            switch (state) {
                case QMediaPlayer::PlayingState: statusText = "▶️ Lecture en cours"; break;
                case QMediaPlayer::PausedState: statusText = "⏸️ En pause"; break;
                case QMediaPlayer::StoppedState: statusText = "⏹️ Arrêté"; break;
            }
            qDebug() << "Player state:" << statusText;
            window->statusBar()->showMessage(statusText);
        });

    // Sélection d'un fichier -> mise à jour infos
    QObject::connect(listWidget, &QListWidget::currentItemChanged,
        [this](QListWidgetItem *current, QListWidgetItem *previous) {
            Q_UNUSED(previous);
            if (current) {
                // Récupère le vrai nom de fichier depuis les data
                QString fileName = current->data(Qt::UserRole).toString();
                if (!fileName.isEmpty()) {
                    updateFileInfo(fileName);
                }
            }
        });

    // Double-clic ou Entrée -> jouer
    QObject::connect(listWidget, &QListWidget::itemActivated,
        [this](QListWidgetItem *item) {
            // Récupère le vrai nom de fichier depuis les data
            QString fileName = item->data(Qt::UserRole).toString();
            if (!fileName.isEmpty()) {
                playSelectedFile(fileName);
            }
        });
    
    // Installation du filtre d'événements pour la navigation clavier
    listWidget->installEventFilter(this);

    central->setLayout(mainLayout);
    window->setCentralWidget(central);
    
    // Style général de la fenêtre
    window->setStyleSheet(
        "QMainWindow { background-color: #1a1a1a; }"
        "QLabel { color: #fff; }"
    );
    
    // Status bar pour afficher les infos
    window->statusBar()->showMessage("🎬 EpiKodi - Prêt");
    window->statusBar()->setStyleSheet("color: #ccc; background-color: #2b2b2b; padding: 4px;");
    
    window->show();
}

void QtUI::closeWindow() {
    if (window) window->close();
}

void QtUI::showMenu(const std::vector<std::string> &mediaFiles) {
    if (!listWidget) return;
    listWidget->clear();
    
    if (mediaFiles.empty()) {
        QListWidgetItem* item = new QListWidgetItem("❌ Aucun fichier média trouvé dans assets/");
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
        item->setForeground(QColor("#999"));
        listWidget->addItem(item);
        
        if (infoLabel) {
            infoLabel->setText("📂 Aucun média trouvé\n\nPlacez vos fichiers vidéo, audio ou image dans le dossier 'assets/' et relancez l'application.");
        }
    } else {
        for (const auto &fileName : mediaFiles) {
            // Crée un MediaFile pour avoir les infos de type
            QString fullPath = QString("assets/") + QString::fromStdString(fileName);
            epikodi::MediaFile mediaFile(fullPath.toStdString());
            
            QListWidgetItem* item = new QListWidgetItem();
            
            // Affichage avec icône selon le type
            QString displayText;
            switch (mediaFile.getMediaType()) {
                case MediaType::VIDEO: displayText = "🎥 " + QString::fromStdString(fileName); break;
                case MediaType::AUDIO: displayText = "🎵 " + QString::fromStdString(fileName); break;
                case MediaType::IMAGE: displayText = "🖼️ " + QString::fromStdString(fileName); break;
                case MediaType::UNKNOWN: displayText = "❓ " + QString::fromStdString(fileName); break;
            }
            
            item->setText(displayText);
            // Stocke le vrai nom de fichier dans les data pour la lecture
            item->setData(Qt::UserRole, QString::fromStdString(fileName));
            
            listWidget->addItem(item);
        }
        
        // Sélectionne le premier élément
        if (listWidget->count() > 0) {
            listWidget->setCurrentRow(0);
        }
        
        if (infoLabel) {
            infoLabel->setText(QString("📁 %1 fichier(s) média trouvé(s)\n\n"
                                     "🔼🔽 Flèches : naviguer\n"
                                     "⏎ Entrée : lancer la lecture\n"
                                     "Double-clic : lancer la lecture")
                              .arg(mediaFiles.size()));
        }
    }
    
    app.exec(); // boucle Qt
}

void QtUI::updateFileInfo(const QString &fileName) {
    if (!infoLabel) return;
    
    QString fullPath = QString("assets/") + fileName;
    QFileInfo fileInfo(fullPath);
    
    if (!fileInfo.exists()) {
        infoLabel->setText("❌ Fichier non trouvé : " + fileName);
        return;
    }
    
    // Crée un MediaFile pour obtenir les infos détaillées
    epikodi::MediaFile mediaFile(fullPath.toStdString());
    
    QString info = QString("📄 %1\n").arg(fileName);
    
    // Type avec icône
    switch (mediaFile.getMediaType()) {
        case MediaType::VIDEO: info += "🎥 Type : Vidéo\n"; break;
        case MediaType::AUDIO: info += "🎵 Type : Audio\n"; break;
        case MediaType::IMAGE: info += "🖼️ Type : Image\n"; break;
        case MediaType::UNKNOWN: info += "❓ Type : Inconnu\n"; break;
    }
    
    info += QString("💾 Taille : %1\n").arg(QString::fromStdString(mediaFile.getSizeFormatted()));
    info += QString("📏 Extension : %1").arg(QString::fromStdString(mediaFile.getExtension()));
    
    infoLabel->setText(info);
}

void QtUI::playSelectedFile(const QString &fileName) {
    QString fullPath = QString("assets/") + fileName;
    
    QFileInfo fileInfo(fullPath);
    if (!fileInfo.exists()) {
        QMessageBox::warning(window, "Fichier non trouvé", 
            QString("Le fichier '%1' n'existe pas.\n\nChemin recherché: %2")
                .arg(fileName)
                .arg(fileInfo.absoluteFilePath()));
        return;
    }
    
    // Détermine le type de fichier
    epikodi::MediaFile mediaFile(fullPath.toStdString());
    
    switch (mediaFile.getMediaType()) {
        case MediaType::IMAGE:
            displayImage(fileName);
            break;
        case MediaType::VIDEO:
        case MediaType::AUDIO:
            displayVideo(fileName);
            break;
        default:
            QMessageBox::information(window, "Type non supporté", 
                "Ce type de fichier n'est pas supporté pour la lecture.");
            break;
    }
}

void QtUI::displayImage(const QString &fileName) {
    QString fullPath = QString("assets/") + fileName;
    QFileInfo fileInfo(fullPath);
    QString absolutePath = fileInfo.absoluteFilePath();
    
    QPixmap pixmap(absolutePath);
    if (pixmap.isNull()) {
        QMessageBox::warning(window, "Erreur image", 
            "Impossible de charger l'image : " + fileName);
        return;
    }
    
    // Cache les contrôles vidéo et affiche l'image
    hideVideoControls();
    
    // Redimensionne l'image pour qu'elle tienne dans le widget
    QSize labelSize = imageLabel->size();
    if (labelSize.width() < 100) labelSize = QSize(800, 600); // Taille par défaut
    
    QPixmap scaledPixmap = pixmap.scaled(labelSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    imageLabel->setPixmap(scaledPixmap);
    
    imageLabel->show();
    videoWidget->hide();
    
    window->statusBar()->showMessage("🖼️ Image affichée : " + fileName);
    
    qDebug() << "Displaying image:" << absolutePath;
}

void QtUI::displayVideo(const QString &fileName) {
    QString fullPath = QString("assets/") + fileName;
    QFileInfo fileInfo(fullPath);
    QString absolutePath = fileInfo.absoluteFilePath();
    
    // Affiche les contrôles vidéo et cache l'image
    showVideoControls();
    
    qDebug() << "Attempting to play video/audio:" << absolutePath;
    
    player->setMedia(QUrl::fromLocalFile(absolutePath));
    player->play();
    
    window->statusBar()->showMessage("▶️ Lecture : " + fileName);
}

void QtUI::showVideoControls() {
    videoWidget->show();
    imageLabel->hide();
}

void QtUI::hideVideoControls() {
    videoWidget->hide();
    // On garde les boutons de contrôle mais on arrête la lecture
    player->stop();
}

bool QtUI::eventFilter(QObject *obj, QEvent *event) {
    if (obj == listWidget && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        
        switch (keyEvent->key()) {
            case Qt::Key_Return:
            case Qt::Key_Enter:
                if (listWidget->currentItem()) {
                    QString fileName = listWidget->currentItem()->data(Qt::UserRole).toString();
                    if (!fileName.isEmpty()) {
                        playSelectedFile(fileName);
                    }
                    return true;
                }
                break;
            case Qt::Key_Space:
                // Bascule play/pause
                if (player->state() == QMediaPlayer::PlayingState) {
                    player->pause();
                } else {
                    player->play();
                }
                return true;
            case Qt::Key_Escape:
                player->stop();
                return true;
        }
    }
    
    return QObject::eventFilter(obj, event);
}

}
