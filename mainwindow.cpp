#include "mainwindow.h"
#include "folderreader.h"
#include "metastructures.h"
#include "./ui_mainwindow.h"

#include "audioengine.h"

#include <iostream>
#include <qlabel.h>
#include <QTimer>

#define SONGROLE Qt::UserRole
#define ALBUMROLE Qt::UserRole
#define ARTISTROLE Qt::UserRole

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPixmap logoPixmap("/home/mmb/Code/Ultrafi/Ultrafi/Covers/resizedlogo");
    ui->LogoLabel->setPixmap(logoPixmap);
    ui->LogoLabel->setScaledContents(true);

    artLibrary.artlibInit();

    ui->coverLabel->setPixmap(artLibrary.albumCovers.front()->albumCover);
    ui->coverLabel->setScaledContents(true);

    readFolder();

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::ScrubTick));
    timer->start(1000);

    for (auto &art : library.artists) {

        if (!art->albums.empty()) {
            QListWidgetItem * artistItem = new QListWidgetItem();
            artistItem->setText(QString::fromStdString(art->name));
            artistItem->setData(ARTISTROLE, QVariant::fromValue(art));
            ui->artistList->addItem(artistItem);
        }

        for (auto &alb : art->albums) {

            if (!alb->songs.empty()) {
                QListWidgetItem * albumItem = new QListWidgetItem();
                albumItem->setText(QString::fromStdString(alb->name));
                albumItem->setData(ALBUMROLE, QVariant::fromValue(alb));
                ui->albumList->addItem(albumItem);
            }

            for (auto &song : alb->songs) {

                QListWidgetItem* songItem = new QListWidgetItem();
                if (song->name == "Unknown Song") {
                    std::filesystem::directory_entry s(song->path);
                    songItem->setText(QString::fromStdString(art->name + " - " + s.path().filename().u8string()));
                }
                else {
                    songItem->setText((QString::fromStdString(art->name + " - " + song->name)));
                }

                songItem->setData(SONGROLE, QVariant::fromValue(song));

                ui->songList->addItem(songItem);
            }
        }
    }
}

void MainWindow::ScrubTick() {
    if (music) {
        ui->ScrubbingSlider->setValue( (int)(((float)music->getPlayPosition() / (float)music->getPlayLength()) * 1000) );

        // Sets time label, ensures leading zero infront of single digit seconds
        std::string seconds = std::to_string((music->getPlayPosition() / 1000) % 60);
        if (seconds.length() < 2) {
            seconds = '0' + seconds;
        }
        std::string time = std::to_string((music->getPlayPosition() / 60000)) + ":" + seconds;
        ui->TimeLabel->setText(QString::fromStdString(time));
    }
    if (curItemRef != NULL) ui->TitleLabel->setText(QString::fromStdString(curSongRef->artistName + " - " + curSongRef->name));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_playButton_clicked()
{
    if (music == NULL || music->getIsPaused()) {
        music = playAudioFile(audioEngine, curSongPath.c_str());
        music->setPlayPosition(songPos);
        ui->playButton->setText("Pause");
    }
    else {
        songPos = music->getPlayPosition();
        audioEngine->setAllSoundsPaused();
        ui->playButton->setText("Play");
    }

}

void MainWindow::on_volumeSlider_valueChanged(int value)
{
    audioEngine->setSoundVolume(((float)value)/99);
}


void MainWindow::on_songList_itemDoubleClicked(QListWidgetItem *item)
{
    Song * itemSong = item->data(SONGROLE).value<Song*>();
    handleSongPlay(itemSong);
    curItemRef = item;
}

void MainWindow::customAlbumSlot(QListWidgetItem * item) {
    Song * songItem = item->data(Qt::UserRole).value<Song*>();
    handleSongPlay(songItem);
    curItemRef = item;
}

void MainWindow::handleSongPlay(Song * itemSong) {
    curSongPath = itemSong->path;

    MainWindow::on_playButton_clicked();
    songPos = 0;
    MainWindow::on_playButton_clicked();

    handleCoverFind(itemSong->albumName);
    curSongRef = itemSong;

    ScrubTick();
}

void MainWindow::handleCoverFind(std::string albumName) {
    bool artworkFound = false;
    for (auto &art : library.artists) {
        for (auto &alb : art->albums) {
            if (alb->name == albumName) {
                artworkFound = true;
                ui->coverLabel->setPixmap(alb->albumArtwork->albumCover);
                goto artworkNotFound;
            }
        }
    }

    artworkNotFound:
        if (!artworkFound) {
            ui->coverLabel->setPixmap(artLibrary.albumCovers.front()->albumCover);
        }

        ui->coverLabel->setScaledContents(true);
}


void MainWindow::on_ScrubbingSlider_actionTriggered(int action)
{
    // 3 is step add 4 is step subtract on the slider 7 is move
    if (action == 7 || action == 3 || action == 4) {
            music->setPlayPosition((float)music->getPlayLength() * ((float)this->ui->ScrubbingSlider->sliderPosition() / 1000));
    }
    ScrubTick();
}

void MainWindow::on_artistList_itemDoubleClicked(QListWidgetItem *item)
{
    Artist * itemArtist = item->data(ARTISTROLE).value<Artist*>();

    QListWidget * newTab = new QListWidget();

    for (auto alb : itemArtist->albums) {
            if (!alb->songs.empty()) {
                QListWidgetItem * newItem = new QListWidgetItem();
                newItem->setText(QString::fromStdString(alb->name));
                newItem->setData(Qt::UserRole, QVariant::fromValue(alb));
                newTab->addItem(newItem);
            }
    }

    connect(newTab, &QListWidget::itemDoubleClicked, this, &MainWindow::customArtistSlot);
    ui->tabWidget->addTab(newTab, QString::fromStdString(itemArtist->name));
    ui->tabWidget->setCurrentWidget(newTab);
}

void MainWindow::on_albumList_itemDoubleClicked(QListWidgetItem *item)
{
    handleAlbumTab(item->data(Qt::UserRole).value<Album*>());
}



void MainWindow::on_tabWidget_tabBarDoubleClicked(int index)
{
    QString widgetName = ui->tabWidget->widget(index)->objectName();
    std::cout << index << std::endl;
    if(widgetName != "ArtistsTab" && widgetName != "AlbumsTab" && widgetName != "SongsTab") {
            ui->tabWidget->removeTab(index);
    }
}

// Handles double click from new artist tab
void MainWindow::customArtistSlot(QListWidgetItem * item) {
    handleAlbumTab(item->data(Qt::UserRole).value<Album*>());
}

void MainWindow::handleAlbumTab(Album * albumItem) {
    QListWidget * newTab = new QListWidget();

    for (auto song : albumItem->songs) {
            QListWidgetItem * newItem = new QListWidgetItem();
            if (song->name == "Unknown Song") {
            std::filesystem::directory_entry s(song->path);
            newItem->setText(QString::fromStdString(std::to_string(song->track) + " - " + s.path().filename().u8string()));
            }
            else {
            newItem->setText((QString::fromStdString(std::to_string(song->track) + " - " + song->name)));
            }
            newItem->setData(Qt::UserRole, QVariant::fromValue(song));
            newTab->addItem(newItem);
    }

    connect(newTab, &QListWidget::itemDoubleClicked, this, &MainWindow::customAlbumSlot);
    ui->tabWidget->addTab(newTab, QString::fromStdString(albumItem->name));
    ui->tabWidget->setCurrentWidget(newTab);

    handleCoverFind(albumItem->name);
}

void MainWindow::on_SkipButton_clicked()
{
    if (curItemRef == NULL) return;

    QListWidget* parent = dynamic_cast<QListWidget*>(curItemRef->listWidget());

    if (parent) {
        int curIndex = parent->row(curItemRef);

        if (curIndex < parent->count() - 1) {
            handleSongPlay(parent->item(curIndex + 1)->data(Qt::UserRole).value<Song*>());
            curItemRef = parent->item(curIndex + 1);
            return;
        }
        // Back to start of list
        else {
            handleSongPlay(parent->item(0)->data(Qt::UserRole).value<Song*>());
            curItemRef = parent->item(0);
            return;
        }
    }
}

void MainWindow::callSkip() {
    on_SkipButton_clicked();
}

void MainWindow::on_BackButton_clicked()
{
    if (curItemRef == NULL) return;
    if (music->getPlayPosition() < 5000) {
        QListWidget* parent = dynamic_cast<QListWidget*>(curItemRef->listWidget());

        if (parent) {
            int curIndex = parent->row(curItemRef);

            if (curIndex > 0) {
                handleSongPlay(parent->item(curIndex - 1)->data(Qt::UserRole).value<Song*>());
                curItemRef = parent->item(curIndex - 1);
                return;
            }
            // End of list
            else {
                handleSongPlay(parent->item(parent->count() - 1)->data(Qt::UserRole).value<Song*>());
                curItemRef = parent->item(parent->count() - 1);
                return;
            }
        }
    }
    else {
        music->setPlayPosition(songPos);
    }

}

// UNUSED SLOTS
void MainWindow::on_ScrubbingSlider_sliderPressed()
{

}

void MainWindow::on_ScrubbingSlider_valueChanged(int value)
{

}

void MainWindow::on_ScrubbingSlider_sliderMoved(int position) {
    //    music->setPlayPosition((float)music->getPlayLength() * ((float)position / 1000));
}

void MainWindow::on_volumeSlider_sliderMoved(int position)
{

}








