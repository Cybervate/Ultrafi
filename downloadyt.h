#ifndef DOWNLOADYT_H
#define DOWNLOADYT_H

#include <QDialog>
#include <iostream>

namespace Ui {
class DownloadYt;
}

class DownloadYt : public QDialog
{
    Q_OBJECT

public:
    explicit DownloadYt(QWidget *parent = nullptr);
    ~DownloadYt();

    void download();
    void setPath(std::string folderPath);

private:
    Ui::DownloadYt *ui;

    std::string path = "";

};

#endif // DOWNLOADYT_H
