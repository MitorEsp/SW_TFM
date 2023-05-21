#include "widget.h"
#include "./ui_widget.h"

#include <QListWidget>

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
  ui->setupUi(this);
  ui->stopButton->setEnabled(false);

  connect(&timer, &QTimer::timeout, this, &Widget::on_going_test);
}

Widget::~Widget() { delete ui; }

void Widget::on_connectButton_clicked() {
  ConnectInteface CnnIf;
  errorConnIntf erCnnIf;
  char server_message[100];

  memset_pattern16(server_message, "\0", sizeof(server_message));

  QString txt = "PC: Connecting with device in IP ";
  QString num;
  uint8_t SERVER_IP[4];

  CnnIf.Get_Server_IP(SERVER_IP);

  txt.append(num.setNum(SERVER_IP[0]))
      .append(".")
      .append(num.setNum(SERVER_IP[1]))
      .append(".");
  txt.append(num.setNum(SERVER_IP[2]))
      .append(".")
      .append(num.setNum(SERVER_IP[3]))
      .append(".");

  ui->listWidget->addItem(txt);

  erCnnIf = CnnIf.Send_command((char *)CMD_IDN, sizeof(CMD_IDN), server_message,
                               sizeof(server_message));

  if (!erCnnIf) {
    ui->listWidget->addItem("PC: Connection is available");
    txt = server_message;
    ui->listWidget->addItem(txt);
  } else {
    ui->listWidget->addItem("PC: Connection is not available");
    ui->listWidget->addItem(stringErrorConnIntf[erCnnIf - 1]);
  }

  erCnnIf = CnnIf.Send_command((char *)CMD_TST, sizeof(CMD_TST), server_message,
                               sizeof(server_message));

  if (!erCnnIf) {
    txt = server_message;
    ui->listWidget->addItem(txt);
  } else {
    ui->listWidget->addItem("PC: Error: ");
    ui->listWidget->addItem(stringErrorConnIntf[erCnnIf - 1]);
  }
}

void Widget::on_initButton_clicked() {
  ConnectInteface CnnIf;
  errorConnIntf erCnnIf;
  char server_message[30];

  erCnnIf = CnnIf.Send_command((char *)CMD_INIT, sizeof(CMD_INIT),
                               server_message, sizeof(server_message));

  if (erCnnIf == ERCI_RECV_MESSAGE || erCnnIf == NO_ERROR) {
    // ui->listWidget->addItem(server_message);
    ui->initButton->setEnabled(false);
    ui->stopButton->setEnabled(true);
    tstStep = 0;
    ui->progressBar->setValue(100 * tstStep / MAX_STEP);
    this->timer.start(1250);
  } else {
    ui->listWidget->addItem("PC: Test not started\nPC: Error: ");
    ui->listWidget->addItem(stringErrorConnIntf[erCnnIf - 1]);
  }
}

void Widget::on_stopButton_clicked() { this->stopTest = true; }

void Widget::stop_test() {
  this->timer.stop();
  ConnectInteface CnnIf;
  errorConnIntf erCnnIf;
  char server_message[30];

  this->timer.stop();
  this->stopTest = false;

  ui->initButton->setEnabled(true);
  ui->stopButton->setEnabled(false);

  erCnnIf = CnnIf.Send_command((char *)CMD_ABOR, sizeof(CMD_ABOR),
                               server_message, sizeof(server_message));

  if (!erCnnIf) {
    ui->listWidget->addItem(server_message);
  } else {
    ui->listWidget->addItem("PC: Error: ");
    ui->listWidget->addItem(stringErrorConnIntf[erCnnIf - 1]);
  }
}

void Widget::on_clearLogButton_clicked() {
  ui->listWidget->clear();
  ui->listWid_test->clear();

  ui->progressBar->setValue(0);
}

/* aca ves enviando los mssg para ir teniendo feedbak y vas analizando l oque
 * llega */
void Widget::on_going_test() {

  ConnectInteface CnnIf;
  errorConnIntf erCnnIf;
  char server_message[2000];
  float buffSamples[256];
  float maxSample = 0;
  double actualFreq = 0;
  char *endptr;
  int indexSam = 0;
  int retries;
  QString txt = "";
  QString num;

  if (this->stopTest) {
    Widget::stop_test();
  } else {


    erCnnIf = CnnIf.Send_command((char *)CMD_UPD, sizeof(CMD_UPD),
                                 server_message, sizeof(server_message));

    if (erCnnIf) {
      for (retries = 0; retries < 8; retries++) {

        erCnnIf = CnnIf.Receive_command(server_message, sizeof(server_message));
        if ((!erCnnIf) && (sizeof(server_message) != 0))
          break;
      }
    }

    if ((!erCnnIf) && (sizeof(server_message) != 0)) {

      actualFreq = strtod(&server_message[4], &endptr);

      if (actualFreq != 0) {

        while (strcmp(endptr, " ") != 0) {

          buffSamples[indexSam] = strtof(endptr, &endptr);
          /* Envolvente */
          if (buffSamples[indexSam] > maxSample)
            maxSample = buffSamples[indexSam];
          indexSam++;

          /* Safety exit */
          if (indexSam == 256) {
            break;
          }
        }

        ui->progressBar->setValue(100 * (++tstStep) / MAX_STEP);

        if (0 == strncmp(server_message, "END", 3)) {
          Widget::stop_test();
        } else /* if (strncmp(server_message, "RUN", 3))*/ {

          erCnnIf = CnnIf.Send_command((char *)CMD_KEEP, sizeof(CMD_KEEP),
                                       server_message, sizeof(server_message));

          txt.append(num.setNum(actualFreq))
              .append("\t")
              .append(num.setNum(maxSample));

          ui->listWid_test->addItem(txt);

          if (!(erCnnIf == ERCI_RECV_MESSAGE || erCnnIf == NO_ERROR)) {
            ui->listWidget->addItem("PC: Can not carry on\nPC: Error: ");
            ui->listWidget->addItem(stringErrorConnIntf[erCnnIf - 1]);
            Widget::stop_test();
          }
        }

      } else {
        Widget::stop_test();
      }
    } else {
      ui->listWidget->addItem("PC: Error: ");
      ui->listWidget->addItem(stringErrorConnIntf[erCnnIf - 1]);
      Widget::stop_test();
    }
  }
}
