#include "mainwindow.h"
#include "folderreader.h"
#include "metastructures.h"
#include "./ui_mainwindow.h"

#include "audioengine.h"

#include <iostream>
#include <qlabel.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    FSmodel = new QFileSystemModel;
//    FSmodel->setRootPath(QDir::rootPath());

//    ui->treeView->setModel(FSmodel);
//    ui->treeView->setRootIndex(FSmodel->index("C:/Users/mmbpr.DESKTOP-DKSGGBP.000/Desktop/UltrafiLibrary"));

    readFolder();

    for (auto &art : library.artists) {
        ui->artistList->addItem((QString::fromStdString(art->name)));
        for (auto &alb : art->albums) {
            ui->albumList->addItem((QString::fromStdString(alb->name)));
            for (auto &song : alb->songs) {
                QListWidgetItem* songItem = new QListWidgetItem();
                songItem->setText((QString::fromStdString(song->name)));
                songItem->setData(Qt::UserRole, song->path.c_str());
                ui->songList->addItem(songItem);
            }
        }
    }

    QPixmap* image = new QPixmap("/home/mmb/Code/Ultrafi/Ultrafi/unknowncoverpng");
    ui->coverLabel->setPixmap(*image);
    ui->coverLabel->setScaledContents(true);
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
    std::cout << item->data(Qt::UserRole).toString().toStdString() << std::endl;
    curSongPath = item->data(Qt::UserRole).toString().toStdString();
    MainWindow::on_playButton_clicked();
    songPos = 0;
    MainWindow::on_playButton_clicked();
}

