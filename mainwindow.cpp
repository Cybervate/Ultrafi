#include "mainwindow.h"
#include "folderreader.h"
#include "metastructures.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    FSmodel = new QFileSystemModel;
    FSmodel->setRootPath(QDir::rootPath());

    ui->treeView->setModel(FSmodel);
    ui->treeView->setRootIndex(FSmodel->index("C:/Users/mmbpr.DESKTOP-DKSGGBP.000/Desktop/UltrafiLibrary"));

    readFolder();
}

MainWindow::~MainWindow()
{
    delete ui;
}

