#include "equalizer.h"
#include "ui_equalizer.h"
#include "audioengine.h"

equalizer::equalizer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::equalizer)
{
    ui->setupUi(this);
    this->setWindowTitle("Equalizer");
}

equalizer::~equalizer()
{
    delete ui;
}

void equalizer::on_pushButton_clicked()
{
    setEq(
        ui->verticalSlider->value(),
        ui->verticalSlider_2->value(),
        ui->verticalSlider_3->value(),
        ui->verticalSlider_4->value(),
        ui->verticalSlider_5->value(),
        ui->verticalSlider_6->value(),
        ui->verticalSlider_7->value(),
        ui->verticalSlider_8->value(),
        ui->verticalSlider_9->value(),
        ui->verticalSlider_10->value()
        );
}

