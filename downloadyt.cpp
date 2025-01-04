#include "downloadyt.h"
#include "ui_downloadyt.h"

#include <iostream>

DownloadYt::DownloadYt(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DownloadYt)
{
    ui->setupUi(this);
    download();
}

DownloadYt::~DownloadYt()
{
    delete ui;
}

void DownloadYt::download()
{
//    system("")
    // TODO Change path to build location

    std::string videoUrl = "https://www.youtube.com/watch?v=GpM4doRFfkc";
    std::string command = "yt-dlp -x --audio-format mp3 " + videoUrl + " -o /home/mmb/Desktop/UltrafiLibrary/%(title)s.mp3";

    system(command.c_str());
}

void DownloadYt::setPath(std::string folderPath) {
    path = folderPath;
}
