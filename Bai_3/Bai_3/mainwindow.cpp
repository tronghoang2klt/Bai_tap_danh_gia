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
    ui->customPlot->xAxis->setLabel("time (s)");
    ui->customPlot->yAxis->setLabel(" cuong do anh sang (lux)");
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
    buffer=serialPort->readLine();
    data_string+=QString::fromStdString(buffer.toStdString());
    if(data_string.contains("\n"))//doc het du lieu
    {
        // du lieu nhan dc co dang "d"+"gia tri lux"+"\n"
        index=data_string.lastIndexOf("d",data_string.length());//tim vi tri cua chu d, d la vi tri bat dau
        data_string.remove(0,index+1);//xoa tu dau den vi tri chu d
        ui->pteReceive->insertPlainText(data_string);//hien thi du lieu nhan lux
        data_string.remove("\n");//xoa ki tu \n
        giatri=data_string.toDouble();//chuyen doi thanh gia tri double
        time=time+1;
        addPoint(time,giatri);
        plot();
        data_string="";
        index=0;
    }
}


