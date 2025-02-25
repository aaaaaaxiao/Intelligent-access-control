#include "open.h"
#include "ui_open.h"

Open::Open(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Open)
{
    ui->setupUi(this);

    QIcon icon(":1.jpg");
    this->setWindowIcon(icon);
    this->setWindowTitle("开门");

    m = new QMovie(":/open.gif");
    ui->label->setMovie(m);
    ui->label->setScaledContents(true);
    m->start();
}

Open::~Open()
{
    delete ui;
}
