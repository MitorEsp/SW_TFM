#include "widget.h"
#include "./ui_widget.h"

#include <QListWidget>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->stopButton->setEnabled(false);

    connect(&timer, &QTimer::timeout, this, &Widget::onGoingTest);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_connectButton_clicked()
{
    ConnectInteface CnnIf;
    errorConnIntf erCnnIf;

    ui->listWidget->addItem("PC: Connecting with 16.0.0.16");

    this->CnnIfPtr = &CnnIf;

    erCnnIf = CnnIf.CheckConnect();

    if(!erCnnIf){
        ui->listWidget->addItem("PC: 16.0.0.16 is available");
    }else{
        ui->listWidget->addItem("PC: 16.0.0.16 is not available");
        ui->listWidget->addItem(stringErrorConnIntf[erCnnIf-1]);
    }
}

void Widget::on_initButton_clicked()
{
    ui->initButton->setEnabled(false);
    ui->stopButton->setEnabled(true);
    ui->progressBar->setValue(0);
    this->timer.start(800);
}

void Widget::on_stopButton_clicked()
{
    ui->initButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
    this->timer.stop();
    this->messageNo = 1;
}

void Widget::on_clearLogButton_clicked()
{
    ui->listWidget->clear();
    this->messageNo = 1;
}

void Widget::onGoingTest()
{
    QString txt = "Mensaje numero: ";
    QString num;
    num.setNum(this->messageNo);
    txt.append(num);
    ui->listWidget->addItem(txt);
    ui->progressBar->setValue(this->messageNo*10);

    if(this->messageNo>9){
        Widget::on_stopButton_clicked();
    }
    this->messageNo++;
}
