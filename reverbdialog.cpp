#include "reverbdialog.h"
#include "ui_reverbdialog.h"

ReverbDialog::ReverbDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReverbDialog)
{
    ui->setupUi(this);

    this->setWindowTitle("Reverb");

}

ReverbDialog::~ReverbDialog()
{
    delete ui;
}
