#ifndef EQUALIZER_H
#define EQUALIZER_H

#include <QDialog>

namespace Ui {
class equalizer;
}

class equalizer : public QDialog
{
    Q_OBJECT

public:
    explicit equalizer(QWidget *parent = nullptr);
    ~equalizer();

private slots:
    void on_pushButton_clicked();

private:
    Ui::equalizer *ui;
};

#endif // EQUALIZER_H
