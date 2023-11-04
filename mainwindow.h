#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QList>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPixmap>

#include "metastructures.h"
#include "audioengine.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_playButton_clicked();

    void on_volumeSlider_sliderMoved(int position);

    void on_songList_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;

    QFileSystemModel *FSmodel = new QFileSystemModel;

    std::string curSongPath = "/home/mmb/Desktop/UltrafiLibrary/Earl Sweatshirt/voir dire/01 - 100 High Street.mp3";
    irrklang::ik_u32 songPos = 0;

    irrklang::ISoundEngine* audioEngine = audioInit();
    irrklang::ISound * music = NULL;

};
#endif // MAINWINDOW_H
