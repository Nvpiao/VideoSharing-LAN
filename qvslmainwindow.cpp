#include "qvslmainwindow.h"
#include "ui_qvslmainwindow.h"

QVSLMainWindow::QVSLMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::QVSLMainWindow)
{
    ui->setupUi(this);
}

QVSLMainWindow::~QVSLMainWindow()
{
    delete ui;
}

