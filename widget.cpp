#include "widget.h"
#include "./ui_widget.h"
#include "connectinteface.h"

#include <QListWidget>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_connectButton_clicked()
{
    QString txt = "PC: Connecting with 16.0.0.16";
    ui->listWidget->addItem(txt);

    ConnectInteface CnnIf;

    if(CnnIf.CheckConnect()){
        ui->listWidget->addItem("PC: 16.0.0.16 is available");
    }else{
        ui->listWidget->addItem("PC: 16.0.0.16 is not available");
    }
}


void Widget::on_initButton_clicked()
{
    QString txt = "Test started";
    ui->listWidget->addItem(txt);
}


void Widget::on_stopButton_clicked()
{
    QString txt = "Test stopped and finished";
    ui->listWidget->addItem(txt);
}


void Widget::on_clearLogButton_clicked()
{
    ui->listWidget->clear();
}

