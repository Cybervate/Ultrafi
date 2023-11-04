#include "mainwindow.h"
#include "folderreader.h"
#include "metastructures.h"
#include "./ui_mainwindow.h"

#include "audioengine.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    FSmodel = new QFileSystemModel;
    FSmodel->setRootPath(QDir::rootPath());

    ui->treeView->setModel(FSmodel);
    ui->treeView->setRootIndex(FSmodel->index("C:/Users/mmbpr.DESKTOP-DKSGGBP.000/Desktop/UltrafiLibrary"));

    readFolder();


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_playButton_clicked()
{
    if (music == NULL || music->getIsPaused()) {
        music = playAudioFile(audioEngine, "/home/mmb/Desktop/UltrafiLibrary/Death Grips/Exmilitary/Death_Grips_-_01_-_Beware.mp3");
        music->setPlayPosition(songPos);
        ui->playButton->setText("Pause");
    }
    else {
        songPos = music->getPlayPosition();
        audioEngine->setAllSoundsPaused();
        ui->playButton->setText("Play");
    }

}

