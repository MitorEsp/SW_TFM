#ifndef WIDGET_H
#define WIDGET_H

#include <QTimer>
#include <QWidget>


#include "connectinteface.h"

#define MAX_STEP 170

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget {
  Q_OBJECT

public:
  Widget(QWidget *parent = nullptr);
  ~Widget();

private slots:

  void on_connectButton_clicked();

  void on_initButton_clicked();

  void on_stopButton_clicked();

  void on_clearLogButton_clicked();

  void on_going_test();

  void stop_test();

private:
  Ui::Widget *ui;
  ConnectInteface *CnnIfPtr;
  QTimer timer;
  bool stopTest = false;
  int tstStep;
};
#endif // WIDGET_H
