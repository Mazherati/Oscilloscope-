#include "dialog.h"
#include "ui_dialog.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <string>
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <QDateTime>
#include <qtimer.h>


dialog::dialog(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::dialog)
{
    ui->setupUi(this);

    ui->temp_graph->addGraph();
    ui->temp_graph->addGraph();
    QPen pen;

    pen.setColor(Qt::red);
    ui->temp_graph->graph(1)->setPen(pen);
    ui->temp_graph->xAxis->setLabel("time(s)");
    ui->temp_graph->yAxis->setLabel("Voltage(V)");
    ui->temp_graph->graph(0)->setScatterStyle(QCPScatterStyle::ssCircle);

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");

    arduino = new QSerialPort(this);
    buffer = "";
    parsed = "";
    temperature = 0.0;

    bool arduino_available = false;
    QString arduino_port_name;

    qDebug() << "Number of available ports: " << QSerialPortInfo::availablePorts().length();
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        qDebug() <<"Has vendor ID: " <<serialPortInfo.hasVendorIdentifier();
        if(serialPortInfo.hasVendorIdentifier())
        {
            qDebug() <<"Vendor ID: " << serialPortInfo.vendorIdentifier();

        }
        qDebug() <<"Has Product ID: " <<serialPortInfo.hasProductIdentifier();
        if(serialPortInfo.hasProductIdentifier())
        {
            qDebug() <<"Product ID: " << serialPortInfo.productIdentifier();

        }
    }

    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        if(serialPortInfo.hasProductIdentifier() && serialPortInfo.hasVendorIdentifier())
        {
            if((serialPortInfo.productIdentifier() == arduino_product_id) && (serialPortInfo.vendorIdentifier() == arduino_vendor_id))
            {
                arduino_available = true;
                arduino_port_name = serialPortInfo.portName();
            }
        }
    }

    if(arduino_available)
    {
        qDebug() << "Arduino port located \n";
        arduino->setPortName(arduino_port_name);
        arduino->open(QSerialPort::ReadOnly);
        arduino->setBaudRate(QSerialPort::Baud115200);
        arduino->setDataBits(QSerialPort::Data8);
        arduino->setFlowControl(QSerialPort::NoFlowControl);
        arduino->setParity(QSerialPort::NoParity);
        arduino->setStopBits(QSerialPort::OneStop);
        QObject::connect(arduino, SIGNAL(readyRead()), this, SLOT(readSerial()));

    }

}


dialog::~dialog()
{
    if(arduino->isOpen())
    {
        arduino->close();
    }
    delete ui;
}


void dialog::readSerial()
{

    QStringList buffer_split = buffer.split("\r\n");
    if(buffer_split.length() < 2)
    {
        serialData = arduino->readAll();
        buffer = buffer + QString::fromStdString(serialData.toStdString());

    }
    else
    {
       qDebug() <<buffer_split;
       temperature = (buffer_split[0].toDouble() * (3.3/4095.0));
       previous = current;
       current = temperature;

       static QTime time(QTime::currentTime());

       double key = time.elapsed()/1000.0;
       static double lastPointKey = 0;
       if (key-lastPointKey > 0.002)
       {

         ui->temp_graph->graph(0)->addData(key, temperature);
         ui->temp_graph->graph(1)->addData(key, trigger_level);


         lastPointKey = key;
       }
       ui->temp_graph->xAxis->setRange(key, xscale, Qt::AlignRight);
       ui->temp_graph->yAxis->setRange(0, yscale);

       if(flag == 1){
       updateTemp(buffer_split[0]);
       trigger();}

       buffer = "";
    }
}


void dialog::updateTemp(const QString sensor_temp)
{
    if(flag == 1){
    ui->lcdNumber->display(sensor_temp.toDouble() *(3.3/4095.0));
    }
}


void dialog::on_pushButton_clicked()
{
   flag = 1;
   ui->temp_graph->setInteraction(QCP::iRangeDrag, false);
   ui->temp_graph->setInteraction(QCP::iRangeZoom, false);
}


void dialog::on_pushButton_2_clicked()
{
    flag = 0;
    ui->temp_graph->setInteraction(QCP::iRangeDrag, true);
    ui->temp_graph->setInteraction(QCP::iRangeZoom, true);
}


void dialog::on_doubleSpinBox_2_valueChanged(double arg1)
{
    trigger_level = arg1;
}


void dialog::on_doubleSpinBox_3_valueChanged(double arg1)
{
    xscale = arg1*10;
}


void dialog::on_doubleSpinBox_4_valueChanged(double arg1)
{
    yscale = arg1;
}


void dialog::trigger()
{
    if(rising_flag == 1)
    {
        if(previous < trigger_level && current > trigger_level)
        {
            ui->temp_graph->replot();
            ui->temp_graph->update();
        }

    }
    else if(falling_flag == 1)
    {
       if(previous > trigger_level && current < trigger_level)
       {
            ui->temp_graph->replot();
            ui->temp_graph->update();
        }
    }

    else if(continuous_flag == 1)
    {
            ui->temp_graph->replot();
            ui->temp_graph->update();
    }
}


void dialog::on_risingButton_3_clicked()
{
   rising_flag = 1;
   falling_flag =0;
   continuous_flag = 0;
   ui->lcdNumber_3->display(rising_flag);
   ui->lcdNumber_4->display(falling_flag);
   ui->lcdNumber_5->display(continuous_flag);
}


void dialog::on_fallingButton_4_clicked()
{
    falling_flag = 1;
    rising_flag = 0;
    continuous_flag = 0;
    ui->lcdNumber_3->display(rising_flag);
    ui->lcdNumber_4->display(falling_flag);
    ui->lcdNumber_5->display(continuous_flag);

}


void dialog::on_continuous_button_clicked()
{
    rising_flag = 0;
    falling_flag = 0;
    continuous_flag = 1;
    ui->lcdNumber_3->display(rising_flag);
    ui->lcdNumber_4->display(falling_flag);
    ui->lcdNumber_5->display(continuous_flag);

}
