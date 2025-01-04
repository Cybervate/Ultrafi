#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QList>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPixmap>
#include <QMediaPlayer>
#include <QFileDialog>
#include <QAudioOutput>

#include "reverbdialog.h"
#include "equalizer.h"
#include "downloadyt.h"

#include "metastructures.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    void callSkip();
//    std::string path = "/home/mmb/Music";
    std::string path = QFileDialog::getExistingDirectory(nullptr, "Select Music Library Folder").toStdString();

    ~MainWindow();

private slots:
    void on_playButton_clicked();

    void on_volumeSlider_sliderMoved(int position);

    void on_songList_itemDoubleClicked(QListWidgetItem *item);

    void on_ScrubbingSlider_sliderMoved(int position);

    void on_ScrubbingSlider_valueChanged(int value);

    void on_ScrubbingSlider_actionTriggered(int action);

    void on_ScrubbingSlider_sliderPressed();

    void on_artistList_itemDoubleClicked(QListWidgetItem *item);

    void on_tabWidget_tabBarDoubleClicked(int index);

    void customArtistSlot(QListWidgetItem * item);

    void customAlbumSlot(QListWidgetItem * item);

    void on_albumList_itemDoubleClicked(QListWidgetItem *item);

    void on_SkipButton_clicked();

    void on_BackButton_clicked();

    void on_volumeSlider_valueChanged(int value);

    void on_ShuffleButton_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_actionReverb_triggered();

    void on_actionEqualizer_triggered();

    void on_ShuffleButton_released();

    void on_actionAdd_From_YouTube_URL_triggered();

    void on_actionOpen_Library_Folder_triggered();

private:
    Ui::MainWindow *ui;

    ReverbDialog * reverbDialog = new ReverbDialog(this);
    equalizer * eqDialog = new equalizer(this);
    DownloadYt * ytdlDialog = new DownloadYt(this);

    QListWidgetItem * curItemRef = NULL;

    QListWidget * curList = NULL;

    std::string curTabName = "";

    QIcon playIcon = QIcon(QPixmap(":/Icons/Icons/play.png"));
    QIcon pauseIcon = QIcon(QPixmap(":/Icons/Icons/pause.png"));
    QIcon forwardIcon = QIcon(QPixmap(":/Icons/Icons/forward.png"));
    QIcon backIcon = QIcon(QPixmap(":/Icons/Icons/forward.png").transformed(QTransform().scale(-1, 1)));
    QIcon shuffleIcon = QIcon(QPixmap(":/Icons/Icons/shuffle.png"));
    QPixmap volumePixmap = QPixmap(":/Icons/Icons/volume.png");
    QPixmap mutePixmap = QPixmap(":/Icons/Icons/mute.png");


//    std::string curSongPath = "/home/mmb/Desktop/UltrafiLibrary/Earl Sweatshirt/voir dire/01 - 100 High Street.mp3";

    void loadLibrary();
    void ScrubTick();
    void handleSongPlay(Song * itemSong);
    void handleCoverFind(Album * album);
    void handleAlbumTab(Album * albumItem);

    void handleSongFinished(QMediaPlayer::MediaStatus status);

};
#endif // MAINWINDOW_H
