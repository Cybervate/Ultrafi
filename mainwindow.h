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
    void callSkip();
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

private:
    Ui::MainWindow *ui;

    QFileSystemModel *FSmodel = new QFileSystemModel;

    Song * curSongRef;
    QListWidgetItem * curItemRef = NULL;

    std::string curSongPath = "/home/mmb/Desktop/UltrafiLibrary/Earl Sweatshirt/voir dire/01 - 100 High Street.mp3";
    irrklang::ik_u32 songPos = 0;

    irrklang::ISoundEngine* audioEngine = audioInit();
    irrklang::ISound * music = NULL;

    void ScrubTick();
    void handleSongPlay(Song * itemSong);
    void handleCoverFind(std::string albumName);
    void handleAlbumTab(Album * albumItem);

};
#endif // MAINWINDOW_H
