#include "widget.h"
#include "./ui_widget.h"

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
    errorConnIntf erCnnIf;

    if(this->CnnIfPtr == NULL){
         ui->listWidget->addItem("PC: Error: Connection not yet initialized");
    }
    else{
        erCnnIf = this->CnnIfPtr->RunTest();
    }

    if(!erCnnIf){
        ui->listWidget->addItem("PC: Test succesfull");
    }else{
        ui->listWidget->addItem("PC: Test failed");
        ui->listWidget->addItem(stringErrorConnIntf[erCnnIf-1]);
    }
}


void Widget::on_stopButton_clicked()
{
    QString txt = "Test stopped and finished";
    ui->listWidget->addItem(txt);
    ui->stopButton->
}


void Widget::on_clearLogButton_clicked()
{
    ui->listWidget->clear();
}



