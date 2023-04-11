#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

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

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
