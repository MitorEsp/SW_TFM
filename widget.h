#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>

#include "connectinteface.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:

    void on_connectButton_clicked();

    void on_initButton_clicked();

    void on_stopButton_clicked();

    void on_clearLogButton_clicked();

    void onGoingTest();

private:
    Ui::Widget *ui;

    ConnectInteface *CnnIfPtr;
    QTimer timer;
    int messageNo = 1;
};
#endif // WIDGET_H
