#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSerialPort>
#include <QByteArray>
#include <QString>
#include <QtMath>
#include <QStringList>
#include <QDebug>
QSerialPort *serialPort;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setScatterStyle(QCPScatterStyle::ssCircle);
    ui->customPlot->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->customPlot->xAxis->setLabel("time");
    ui->customPlot->yAxis->setLabel("lux");
    ui->customPlot->xAxis->setRange(0,300);
    ui->customPlot->yAxis->setRange(0,300);
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom| QCP::iSelectPlottables);


    data_string="";
    serialPort = new QSerialPort(this);

    //cau hinh cong COM
    serialPort->setPortName("com4");
    serialPort->setBaudRate(QSerialPort::Baud9600);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);

    // mo cong com
    serialPort->open(QIODevice::ReadWrite);
    connect(serialPort,SIGNAL(readyRead()),this,SLOT(serialport_read()));


}

MainWindow::~MainWindow()
{
    delete ui;
    // dong cong com
    serialPort->close();
}

void MainWindow::addPoint(double x,double y)
{
    qv_x.append(x);
    qv_y.append(y);
}
void MainWindow::plot()
{
    ui->customPlot->graph(0)->setData(qv_x,qv_y);
    ui->customPlot->rescaleAxes();
    ui->customPlot->replot();
    ui->customPlot->update();
}


void MainWindow::serialport_read()
{

    QStringList bufferSplit = data_string.split("\n");
    if(bufferSplit.length()<2)
    {
        buffer=serialPort->readAll();
        data_string+=QString::fromStdString(buffer.toStdString());
    }
    else
    {
        ui->pteReceive->insertPlainText(bufferSplit[0]);
        // show data read ok! eg: 96,80,94
        qDebug() << "bufferSplit[0]:" << bufferSplit[0];

        giatri=QString(bufferSplit[0]).toDouble();// not working
        //giatri++;
        time=time+1;
        addPoint(time,giatri);
        plot();
        data_string="";
        bufferSplit={""};
    }
}


