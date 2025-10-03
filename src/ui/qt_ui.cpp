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
#include <QPushButton>
#include <QStackedWidget>
#include <QGridLayout>

namespace epikodi {

QtUI::QtUI(int &argc, char **argv)
    : app(argc, argv), window(nullptr), menuWidget(nullptr),
      videoBtn(nullptr), audioBtn(nullptr), imageBtn(nullptr),
      contentStack(nullptr), videoPage(nullptr), audioPage(nullptr),
      imagePage(nullptr), listWidget(nullptr), player(nullptr),
      videoWidget(nullptr), infoLabel(nullptr), imageLabel(nullptr),
      rightPanel(nullptr) {}

QtUI::~QtUI() {
    delete player;
    delete window;
}

void QtUI::openWindow(const std::string &title) {
    window = new QMainWindow();
    window->setWindowTitle(QString::fromStdString(title));
    window->resize(1400, 900);

    // Applique le thème Epitech
    applyEpitechTheme();

    QWidget *central = new QWidget(window);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Crée le menu principal
    createMainMenu();
    mainLayout->addWidget(menuWidget);

    // Crée le conteneur pour les différentes sections
    contentStack = new QStackedWidget();
    
    // Crée les pages pour chaque section
    createVideoPage();
    createAudioPage();
    createImagePage();
    
    contentStack->addWidget(videoPage);
    contentStack->addWidget(audioPage);
    contentStack->addWidget(imagePage);
    
    mainLayout->addWidget(contentStack, 1); // stretch factor 1 pour prendre l'espace restant

    central->setLayout(mainLayout);
    window->setCentralWidget(central);
    
    // Status bar Epitech
    window->statusBar()->showMessage("EpiKodi v2.0 - Ready");
    window->statusBar()->setStyleSheet(
        "QStatusBar { "
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #1e3a8a, stop:1 #3b82f6); "
        "  color: white; "
        "  padding: 4px; "
        "  border-top: 2px solid #60a5fa; "
        "}"
    );
    
    // Démarre sur la section Vidéo
    switchToSection("video");
    
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

void QtUI::createMainMenu() {
    menuWidget = new QWidget();
    menuWidget->setFixedHeight(80);
    
    QHBoxLayout *menuLayout = new QHBoxLayout(menuWidget);
    menuLayout->setSpacing(20);
    menuLayout->setContentsMargins(20, 10, 20, 10);
    
    // Logo/Titre EpiKodi
    QLabel *logo = new QLabel("EpiKodi");
    logo->setStyleSheet(
        "QLabel { "
        "  font-size: 24px; "
        "  font-weight: bold; "
        "  color: #60a5fa; "
        "  margin-right: 40px; "
        "}"
    );
    menuLayout->addWidget(logo);
    
    // Style des boutons du menu principal
    QString menuBtnStyle = 
        "QPushButton { "
        "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #3b82f6, stop:1 #1e40af); "
        "  color: white; "
        "  border: 2px solid #60a5fa; "
        "  border-radius: 15px; "
        "  font-size: 16px; "
        "  font-weight: bold; "
        "  padding: 15px 30px; "
        "  min-width: 120px; "
        "} "
        "QPushButton:hover { "
        "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #60a5fa, stop:1 #2563eb); "
        "  border-color: #93c5fd; "
        "} "
        "QPushButton:pressed { "
        "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #1e40af, stop:1 #1e3a8a); "
        "} "
        "QPushButton:checked { "
        "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #1e40af, stop:1 #1e3a8a); "
        "  border-color: #93c5fd; "
        "}";
    
    // Boutons de navigation
    videoBtn = new QPushButton("🎥 Vidéo");
    videoBtn->setCheckable(true);
    videoBtn->setStyleSheet(menuBtnStyle);
    
    audioBtn = new QPushButton("🎵 Audio");
    audioBtn->setCheckable(true);
    audioBtn->setStyleSheet(menuBtnStyle);
    
    imageBtn = new QPushButton("🖼️ Image");
    imageBtn->setCheckable(true);
    imageBtn->setStyleSheet(menuBtnStyle);
    
    // Connexions
    QObject::connect(videoBtn, &QPushButton::clicked, [this]() { switchToSection("video"); });
    QObject::connect(audioBtn, &QPushButton::clicked, [this]() { switchToSection("audio"); });
    QObject::connect(imageBtn, &QPushButton::clicked, [this]() { switchToSection("image"); });
    
    menuLayout->addWidget(videoBtn);
    menuLayout->addWidget(audioBtn);
    menuLayout->addWidget(imageBtn);
    menuLayout->addStretch(); // Pousse le contenu vers la gauche
    
    // Bouton paramètres (pour plus tard)
    QPushButton *settingsBtn = new QPushButton("⚙️");
    settingsBtn->setStyleSheet(
        "QPushButton { "
        "  background: transparent; "
        "  color: #60a5fa; "
        "  border: 2px solid #60a5fa; "
        "  border-radius: 20px; "
        "  font-size: 16px; "
        "  padding: 10px; "
        "  min-width: 40px; "
        "  min-height: 40px; "
        "} "
        "QPushButton:hover { "
        "  background: #60a5fa; "
        "  color: white; "
        "}"
    );
    menuLayout->addWidget(settingsBtn);
}

void QtUI::createVideoPage() {
    videoPage = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(videoPage);
    
    // Panel gauche pour la liste
    QWidget *leftPanel = new QWidget();
    leftPanel->setMaximumWidth(400);
    leftPanel->setMinimumWidth(300);
    
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    
    QLabel *title = new QLabel("Bibliothèque Vidéo");
    title->setStyleSheet(
        "QLabel { "
        "  font-size: 18px; "
        "  font-weight: bold; "
        "  color: #60a5fa; "
        "  padding: 10px; "
        "  border-bottom: 2px solid #3b82f6; "
        "}"
    );
    leftLayout->addWidget(title);
    
    // Liste des vidéos (on réutilise listWidget)
    listWidget = new QListWidget();
    listWidget->setStyleSheet(
        "QListWidget { "
        "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #1e3a8a, stop:1 #1e40af); "
        "  color: white; "
        "  border: 2px solid #3b82f6; "
        "  border-radius: 10px; "
        "  font-size: 14px; "
        "  selection-background-color: #60a5fa; "
        "} "
        "QListWidget::item { "
        "  padding: 12px; "
        "  border-bottom: 1px solid #3b82f6; "
        "} "
        "QListWidget::item:hover { "
        "  background-color: rgba(96, 165, 250, 0.3); "
        "}"
    );
    leftLayout->addWidget(listWidget);
    
    layout->addWidget(leftPanel);
    
    // Panel droit pour la lecture
    rightPanel = new QWidget();
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
    
    // Zone vidéo
    videoWidget = new QVideoWidget();
    videoWidget->setStyleSheet(
        "QVideoWidget { "
        "  background-color: black; "
        "  border: 3px solid #3b82f6; "
        "  border-radius: 10px; "
        "}"
    );
    rightLayout->addWidget(videoWidget);
    
    // Zone image (cachée par défaut pour la page vidéo)
    imageLabel = new QLabel();
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setStyleSheet(
        "QLabel { "
        "  background-color: black; "
        "  border: 3px solid #3b82f6; "
        "  border-radius: 10px; "
        "  color: white; "
        "  font-size: 16px; "
        "}"
    );
    imageLabel->setText("Sélectionnez une vidéo pour la lire");
    imageLabel->setMinimumSize(400, 300);
    imageLabel->hide();
    rightLayout->addWidget(imageLabel);
    
    // Contrôles de lecture
    QWidget *controlsWidget = new QWidget();
    QHBoxLayout *controlsLayout = new QHBoxLayout(controlsWidget);
    
    QString controlBtnStyle = 
        "QPushButton { "
        "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #3b82f6, stop:1 #1e40af); "
        "  color: white; "
        "  border: 2px solid #60a5fa; "
        "  border-radius: 8px; "
        "  font-size: 14px; "
        "  font-weight: bold; "
        "  padding: 10px 20px; "
        "} "
        "QPushButton:hover { "
        "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #60a5fa, stop:1 #2563eb); "
        "} "
        "QPushButton:pressed { "
        "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #1e40af, stop:1 #1e3a8a); "
        "}";
    
    QPushButton *playBtn = new QPushButton("▶️ Play");
    QPushButton *pauseBtn = new QPushButton("⏸️ Pause");
    QPushButton *stopBtn = new QPushButton("⏹️ Stop");
    
    playBtn->setStyleSheet(controlBtnStyle);
    pauseBtn->setStyleSheet(controlBtnStyle);
    stopBtn->setStyleSheet(controlBtnStyle);
    
    controlsLayout->addWidget(playBtn);
    controlsLayout->addWidget(pauseBtn);
    controlsLayout->addWidget(stopBtn);
    controlsLayout->addStretch();
    
    rightLayout->addWidget(controlsWidget);
    layout->addWidget(rightPanel);
    
    // Initialise le player
    player = new QMediaPlayer(window);
    player->setVideoOutput(videoWidget);
    
    // Connexions
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
    
    // Gestion des événements de liste
    QObject::connect(listWidget, &QListWidget::currentItemChanged,
        [this](QListWidgetItem *current, QListWidgetItem *previous) {
            Q_UNUSED(previous);
            if (current) {
                QString fileName = current->data(Qt::UserRole).toString();
                if (!fileName.isEmpty()) {
                    updateFileInfo(fileName);
                }
            }
        });

    QObject::connect(listWidget, &QListWidget::itemActivated,
        [this](QListWidgetItem *item) {
            QString fileName = item->data(Qt::UserRole).toString();
            if (!fileName.isEmpty()) {
                playSelectedFile(fileName);
            }
        });
    
    listWidget->installEventFilter(this);
}

void QtUI::createAudioPage() {
    audioPage = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(audioPage);
    
    QLabel *title = new QLabel("Bibliothèque Audio - En cours de développement");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet(
        "QLabel { "
        "  font-size: 24px; "
        "  font-weight: bold; "
        "  color: #60a5fa; "
        "  padding: 50px; "
        "}"
    );
    layout->addWidget(title);
}

void QtUI::createImagePage() {
    imagePage = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(imagePage);
    
    QLabel *title = new QLabel("Bibliothèque Images - En cours de développement");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet(
        "QLabel { "
        "  font-size: 24px; "
        "  font-weight: bold; "
        "  color: #60a5fa; "
        "  padding: 50px; "
        "}"
    );
    layout->addWidget(title);
}

void QtUI::switchToSection(const QString &section) {
    // Déselectionne tous les boutons
    videoBtn->setChecked(false);
    audioBtn->setChecked(false);
    imageBtn->setChecked(false);
    
    // Active le bouton sélectionné et change de page
    if (section == "video") {
        videoBtn->setChecked(true);
        contentStack->setCurrentWidget(videoPage);
        window->statusBar()->showMessage("Section Vidéo - EpiKodi v2.0");
    } else if (section == "audio") {
        audioBtn->setChecked(true);
        contentStack->setCurrentWidget(audioPage);
        window->statusBar()->showMessage("Section Audio - EpiKodi v2.0");
    } else if (section == "image") {
        imageBtn->setChecked(true);
        contentStack->setCurrentWidget(imagePage);
        window->statusBar()->showMessage("Section Images - EpiKodi v2.0");
    }
}

void QtUI::applyEpitechTheme() {
    window->setStyleSheet(
        "QMainWindow { "
        "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #0f172a, stop:1 #1e293b); "
        "} "
        "QWidget { "
        "  background: transparent; "
        "  color: white; "
        "}"
    );
}

}
