#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QList>

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

private:
    Ui::MainWindow *ui;

    QFileSystemModel *FSmodel = new QFileSystemModel;

    irrklang::ik_u32 songPos = 0;

    irrklang::ISoundEngine* audioEngine = audioInit();
    irrklang::ISound * music = NULL;

};
#endif // MAINWINDOW_H
