#ifndef REVERBDIALOG_H
#define REVERBDIALOG_H

#include <QDialog>

namespace Ui {
class ReverbDialog;
}

class ReverbDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReverbDialog(QWidget *parent = nullptr);
    ~ReverbDialog();

private:
    Ui::ReverbDialog *ui;
};

#endif // REVERBDIALOG_H
