#include "mainwindow.h"
#include "folderreader.h"
#include "metastructures.h"
#include "./ui_mainwindow.h"

#include "audioengine.h"
#include "scrapecovers.h"

#include <iostream>
#include <qlabel.h>

#define SONGROLE 4

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    FSmodel = new QFileSystemModel;
//    FSmodel->setRootPath(QDir::rootPath());

//    ui->treeView->setModel(FSmodel);
//    ui->treeView->setRootIndex(FSmodel->index("C:/Users/mmbpr.DESKTOP-DKSGGBP.000/Desktop/UltrafiLibrary"));
    clearCoverFolder();

    artLibrary.artlibInit();

    readFolder();

    for (auto &art : library.artists) {
        ui->artistList->addItem((QString::fromStdString(art->name)));
        for (auto &alb : art->albums) {
            ui->albumList->addItem((QString::fromStdString(alb->name)));
            for (auto &song : alb->songs) {
                QListWidgetItem* songItem = new QListWidgetItem();
                songItem->setText((QString::fromStdString(song->name)));
//                songItem->setData(Qt::UserRole, song->path.c_str());
                songItem->setData(SONGROLE, QVariant::fromValue(song));
                ui->songList->addItem(songItem);
            }
        }
    }
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


void MainWindow::on_volumeSlider_sliderMoved(int position)
{
    audioEngine->setSoundVolume(((float)position)/99);
}


void MainWindow::on_songList_itemDoubleClicked(QListWidgetItem *item)
{
    Song * itemSong = item->data(SONGROLE).value<Song*>();

    curSongPath = itemSong->path;

    MainWindow::on_playButton_clicked();
    songPos = 0;
    MainWindow::on_playButton_clicked();

    bool artworkFound = false;
    for (auto &art : library.artists) {
        for (auto &alb : art->albums) {
            if (alb->name == itemSong->albumName) {
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

