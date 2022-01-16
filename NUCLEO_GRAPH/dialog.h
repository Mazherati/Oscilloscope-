#ifndef DIALOG_H
#define DIALOG_H

#include <QMainWindow>
#include <QDialog>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QByteArray>
#include <qtimer.h>
#include "mythread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class dialog; }
QT_END_NAMESPACE

class dialog : public QMainWindow
{
    Q_OBJECT

public:
    dialog(QWidget *parent = nullptr);
    ~dialog();


    void trigger();

    MyThread *mthread;



private slots:
    void readSerial();
    void updateTemp(const QString sensor_temp);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_doubleSpinBox_2_valueChanged(double arg1);

    void on_doubleSpinBox_3_valueChanged(double arg1);

    void on_doubleSpinBox_4_valueChanged(double arg1);

    void on_pushButton_3_clicked();

    void on_risingButton_3_clicked();

    void on_fallingButton_4_clicked();

    void on_continuous_button_clicked();

private:
    Ui::dialog *ui;

    QSerialPort *arduino;
    static const quint16 arduino_vendor_id = 1155;
    static const quint16 arduino_product_id = 14155;
    QByteArray serialData;
    QString buffer;
    QString parsed;
    double temperature;
    double f_temp, trigger_offset, trigger_level ,xscale=8 ,yscale=5, current=0, previous=0;
    bool flag = 0, trigger_flag = 0;
    double rising_flag = 0, falling_flag=0, continuous_flag = 0;
    QTimer datatimer;

};


#endif // DIALOG_H
