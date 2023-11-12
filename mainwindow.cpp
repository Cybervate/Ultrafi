#include "mainwindow.h"
#include "folderreader.h"
#include "metastructures.h"
#include "./ui_mainwindow.h"

#include "audioengine.h"

#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <qlabel.h>
#include <QTimer>
#include <bits/stdc++.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Ultrafi");

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

    qAudioInit();

    connect(mediaPlayer, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::handleSongFinished);

    for (auto &art : library.artists) {

        if (!art->albums.empty()) {
            QListWidgetItem * artistItem = new QListWidgetItem();
            artistItem->setText(QString::fromStdString(art->name));
            artistItem->setData(Qt::UserRole, QVariant::fromValue(art));
            ui->artistList->addItem(artistItem);
        }

        for (auto &alb : art->albums) {

            if (!alb->songs.empty()) {
                QListWidgetItem * albumItem = new QListWidgetItem();
                albumItem->setText(QString::fromStdString(alb->name));
                albumItem->setData(Qt::UserRole, QVariant::fromValue(alb));
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

                songItem->setData(Qt::UserRole, QVariant::fromValue(song));

                ui->songList->addItem(songItem);
            }
        }
    }

    curSongRef = library.artists.front()->albums.front()->songs.front();

}

void MainWindow::ScrubTick() {

    ui->ScrubbingSlider->setValue( (int)(((float)mediaPlayer->position() / (float)mediaPlayer->duration()) * 1000));

    std::string seconds = std::to_string((mediaPlayer->position() / 1000) % 60);
    if (seconds.length() < 2) {
        seconds = '0' + seconds;
    }
    std::string time = std::to_string((mediaPlayer->position() / 60000)) + ":" + seconds;
    ui->TimeLabel->setText(QString::fromStdString(time));

    if (curSongRef != NULL) {
        ui->TitleLabel->setText(QString::fromStdString(curSongRef->artistName + " - " + curSongRef->name));
    }

    ui->ShuffleButton->setText("Shuffle " + ui->tabWidget->tabText(ui->tabWidget->currentIndex()));

    if (shuffle && !curTabName.empty()) {
        ui->ShuffleLabel->setText("Currently Shuffling: " + QString::fromStdString(curTabName));
    }
    else {
        ui->ShuffleLabel->setText("");
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_playButton_clicked()
{
    if (!mediaPlayer->isPlaying()) {

        if (curSongRef == NULL) {
            curSongRef = library.artists.front()->albums.front()->songs.front();
        }

        if (mediaPlayer->hasAudio()) {
            mediaPlayer->play();
        }
        else {
            handleSongPlay(curSongRef);
        }


        ui->playButton->setText("Pause");
    }
    else {
        mediaPlayer->pause();
        ui->playButton->setText("Play");
    }

}

void MainWindow::on_volumeSlider_valueChanged(int value)
{
    audioOutput->setVolume((float)value/99);
}


void MainWindow::on_songList_itemDoubleClicked(QListWidgetItem *item)
{
    shuffle = false;
    Song * songItem = item->data(Qt::UserRole).value<Song*>();
    handleSongPlay(songItem);
    curItemRef = item;
}

void MainWindow::customAlbumSlot(QListWidgetItem * item) {
    shuffle = false;
    Song * songItem = item->data(Qt::UserRole).value<Song*>();
    handleSongPlay(songItem);
    curItemRef = item;
}

void MainWindow::handleSongPlay(Song * itemSong) {

    curSongRef = itemSong;

    qPlayFile(itemSong->path);
    mediaPlayer->play();

    ui->playButton->setText("Pause");

    ScrubTick();
}

void MainWindow::handleCoverFind(Album * album) {
    if (!album->albumArtwork->albumCover.size().isEmpty()) {
        ui->coverLabel->setPixmap(album->albumArtwork->albumCover);
    }
    else {
        ui->coverLabel->setPixmap(artLibrary.albumCovers.front()->albumCover);
    }
}


void MainWindow::on_ScrubbingSlider_actionTriggered(int action)
{
     // 3 is step , 4 is step subtract, on the slider 7 is move
    if (action == 7 || action == 3 || action == 4) {
        mediaPlayer->setPosition((float)mediaPlayer->duration() * ((float)this->ui->ScrubbingSlider->sliderPosition() / 1000));
    }
    ScrubTick();
}

void MainWindow::on_artistList_itemDoubleClicked(QListWidgetItem *item)
{
    Artist * itemArtist = item->data(Qt::UserRole).value<Artist*>();

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
    newTab->setAlternatingRowColors(true);
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

    newTab->setAlternatingRowColors(true);

    ui->tabWidget->addTab(newTab, QString::fromStdString(albumItem->name));
    ui->tabWidget->setCurrentWidget(newTab);

    handleCoverFind(albumItem);
}

void MainWindow::on_SkipButton_clicked()
{
    if (curSongRef == NULL) return;
    if (curItemRef == NULL) {
            // TODO should make this finish currnt song
            return;
    }

    if (shuffle) {
        if (shuffleQueueIndex + 1 >= shuffleQueue.size()) {
            std::shuffle(shuffleQueue.begin(), shuffleQueue.end(), std::default_random_engine(rand()));
            shuffleQueueIndex = 0;
        }
        else {
            shuffleQueueIndex++;
        }

        handleSongPlay(shuffleQueue[shuffleQueueIndex]);
        return;
    }

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
    if (mediaPlayer->position() < 5000) {

        if (shuffle) {
            if (shuffleQueue.size() <= 1) {
                return;
            }

            if (shuffleQueueIndex <= 0) {
                std::shuffle(shuffleQueue.begin(), shuffleQueue.end(), std::default_random_engine(rand()));
                shuffleQueueIndex = shuffleQueue.size() - 1;
            }
            else {
                shuffleQueueIndex--;
            }

            handleSongPlay(shuffleQueue[shuffleQueueIndex]);
            return;
        }

        if (curItemRef == NULL) return;

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
        if (curItemRef == NULL) return;
        mediaPlayer->setPosition(0);
    }
    ScrubTick();
}

void MainWindow::on_ShuffleButton_clicked()
{
    shuffle = true;

    curTabName = ui->tabWidget->tabText(ui->tabWidget->currentIndex()).toStdString();

    QListWidget * currentTabList;

    std::string tabType = ui->tabWidget->currentWidget()->metaObject()->className();

    // One of the original tabs: Artists, Albums, Songs
    if (tabType == "QWidget") {
        currentTabList = qobject_cast<QListWidget*>(ui->tabWidget->currentWidget()->findChild<QListWidget*>());
    }
    // New tab created by user click
    else if (tabType == "QListWidget") {
        currentTabList = qobject_cast<QListWidget*>(ui->tabWidget->currentWidget());
    }
    // Should not happen
    else {
        return;
    }

    // Still on same tab
    if (curList == currentTabList) {

        if (shuffleQueueIndex + 1 >= shuffleQueue.size()) {
            std::shuffle(shuffleQueue.begin(), shuffleQueue.end(), std::default_random_engine(rand()));
            shuffleQueueIndex = 0;
        }
        else {
            shuffleQueueIndex++;
        }

        handleSongPlay(shuffleQueue[shuffleQueueIndex]);
    }
    else {
        curList = currentTabList;

        std::string listType = currentTabList->item(0)->data(Qt::UserRole).typeName();

        shuffleQueue.clear();

        if (listType == "Song*") {
            for (int i = 0; i < currentTabList->count(); i++) {
                shuffleQueue.push_back(currentTabList->item(i)->data(Qt::UserRole).value<Song*>());
            }
        }
        else if(listType == "Album*") {
            for (int i = 0; i < currentTabList->count(); i++) {
                for (auto song : currentTabList->item(i)->data(Qt::UserRole).value<Album*>()->songs) {
                    shuffleQueue.push_back(song);
                }
            }
        }
        else if (listType == "Artist*") {
            for (int i = 0; i < currentTabList->count(); i++) {
                for (auto alb : currentTabList->item(i)->data(Qt::UserRole).value<Artist*>()->albums) {
                    for (auto song : alb->songs) {
                        shuffleQueue.push_back(song);
                    }
                }
            }
        }
        std::shuffle(shuffleQueue.begin(), shuffleQueue.end(), std::default_random_engine(rand()));

        shuffleQueueIndex = 0;

        handleSongPlay(shuffleQueue[shuffleQueueIndex]);


    }
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    ScrubTick();
}

void MainWindow::on_actionReverb_triggered()
{
    reverbDialog->show();
}

void MainWindow::handleSongFinished(QMediaPlayer::MediaStatus status) {
    if (status == QMediaPlayer::EndOfMedia) {
        callSkip();
    }
}

void MainWindow::on_actionEqualizer_triggered()
{
    eqDialog->show();
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
