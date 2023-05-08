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
    char server_message[100];

    memset_pattern16(server_message, "\0", sizeof(server_message));

    QString txt = "PC: Connecting with device in IP ";
    QString num;
    uint8_t SERVER_IP[4];

    CnnIf.Get_Server_IP(SERVER_IP);

    txt.append(num.setNum(SERVER_IP[0])).append(".").append(num.setNum(SERVER_IP[1])).append(".");
    txt.append(num.setNum(SERVER_IP[2])).append(".").append(num.setNum(SERVER_IP[3])).append(".");

    ui->listWidget->addItem(txt);

    erCnnIf = CnnIf.Send_command((char*)CMD_IDN, sizeof(CMD_IDN), server_message, sizeof(server_message));

    if(!erCnnIf){
        ui->listWidget->addItem("PC: Connection is available");
        txt = server_message;
        ui->listWidget->addItem(txt);
    }else{
        ui->listWidget->addItem("PC: Connection is not available");
        ui->listWidget->addItem(stringErrorConnIntf[erCnnIf-1]);
    }

    erCnnIf = CnnIf.Send_command((char*)CMD_TST, sizeof(CMD_TST), server_message, sizeof(server_message));

    if(!erCnnIf){
        txt = server_message;
        ui->listWidget->addItem(txt);
    }else{
        ui->listWidget->addItem("PC: Error: ");
        ui->listWidget->addItem(stringErrorConnIntf[erCnnIf-1]);
    }


}

void Widget::on_initButton_clicked()
{
    ConnectInteface CnnIf;
    errorConnIntf erCnnIf;
    char server_message[30];

    erCnnIf = CnnIf.Send_command((char*)CMD_INIT, sizeof(CMD_INIT), server_message, sizeof(server_message));

    if(!erCnnIf){
        ui->listWidget->addItem(server_message);
        ui->initButton->setEnabled(false);
        ui->stopButton->setEnabled(true);
        ui->progressBar->setValue(0);
        this->timer.start(400);
    }else{
        ui->listWidget->addItem("PC: Test not started\nPC: Error: ");
        ui->listWidget->addItem(stringErrorConnIntf[erCnnIf-1]);
    }
}

void Widget::on_stopButton_clicked()
{
    this->timer.stop();
    ConnectInteface CnnIf;
    errorConnIntf erCnnIf;
    char server_message[30];

    ui->initButton->setEnabled(true);
    ui->stopButton->setEnabled(false);

    erCnnIf = CnnIf.Send_command((char*)CMD_ABOR, sizeof(CMD_ABOR), server_message, sizeof(server_message));

    if(!erCnnIf){
        ui->listWidget->addItem(server_message);
    }else{
        ui->listWidget->addItem("PC: Error: ");
        ui->listWidget->addItem(stringErrorConnIntf[erCnnIf-1]);
    }

}

void Widget::on_clearLogButton_clicked()
{
    ui->listWidget->clear();
    this->messageNo = 1;
    ui->progressBar->setValue(0);
}


/* aca ves enviando los mssg para ir teniendo feedbak y vas analizando l oque llega */
void Widget::onGoingTest()
{
//    QString txt = "Mensaje numero: ";
//    QString num;
//    num.setNum(this->messageNo);
//    txt.append(num);
//    ui->listWidget->addItem(txt);
//    ui->progressBar->setValue(this->messageNo*10/177);
//
//    if(this->messageNo>177){
//        Widget::on_stopButton_clicked();
//    }
//    this->messageNo++;

    ConnectInteface CnnIf;
    errorConnIntf erCnnIf;
    char server_message[1600];
    float buffSamples[255];
    int actualFreq = 0;
    char *endptr;
    int indexSam = 0;

    erCnnIf = CnnIf.Send_command((char*)CMD_UPD, sizeof(CMD_UPD), server_message, sizeof(server_message));

    if(!erCnnIf){

        actualFreq = strtol(&server_message[4], &endptr, 10);

        while((endptr==0) || (indexSam>255) ){

            buffSamples[indexSam] = strtof(endptr, &endptr);
            indexSam++;
        }

        if(strncmp(server_message,"END",3)){
            Widget::on_stopButton_clicked();

        }else{


        }

    }else{
        ui->listWidget->addItem("PC: Error: ");
        ui->listWidget->addItem(stringErrorConnIntf[erCnnIf-1]);
    }


    /*
Opcion A) el máximo y le guardas,
Opcion B) FFT.


Enfuncion de la frecuencia actualizas el progres bar.

!hay que crear un nuevo pantallo para ir mostrando los datos!

¡nuevo boton para guardar en un archivo los datos del pantallo!
 */



}
