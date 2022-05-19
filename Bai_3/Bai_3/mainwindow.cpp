

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#  include <QDesktopWidget>
#endif
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>
#include <QtSerialPort>
QSerialPort *serialPort;
MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  setGeometry(400, 250, 542, 390);
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

  setupRealtimeDataDemo(ui->customPlot);
  setWindowTitle(demoName);
  statusBar()->clearMessage();

  ui->customPlot->replot();

}




void MainWindow::setupRealtimeDataDemo(QCustomPlot *customPlot)
{
  demoName = " do thi cuong do anh sang";


  customPlot->addGraph(); // blue line
  customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));


  QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
  timeTicker->setTimeFormat("%h:%m:%s");
  customPlot->xAxis->setTicker(timeTicker);
  customPlot->axisRect()->setupFullAxesBox();
  customPlot->yAxis->setRange(0, 500);

  // make left and bottom axes transfer their ranges to right and top axes:
  connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
  connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

  // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
  connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
  dataTimer.start(0); // Interval 0 means to refresh as fast as possible
}



void MainWindow::realtimeDataSlot()
{
  static QTime timeStart = QTime::currentTime();
  // calculate  new data points:
  double key = timeStart.msecsTo(QTime::currentTime())/1000.0; // time elapsed since start of demo, in seconds
  static double lastPointKey = 0;
  if (key-lastPointKey > 0.002) // at most add point every 2 ms
  {
    // add data to lines:
    ui->customPlot->graph(0)->addData(key,giatri);

    lastPointKey = key;
  }
  // make key axis range scroll with the data (at a constant range size of 8):
  ui->customPlot->xAxis->setRange(key, 8, Qt::AlignRight);
  ui->customPlot->replot();

  // calculate frames per second:
  static double lastFpsKey;
  static int frameCount;
  ++frameCount;
  if (key-lastFpsKey > 2) // average fps over 2 seconds
  {
    ui->statusBar->showMessage(
          QString("%1 FPS, Total Data points: %2")
          .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
          .arg(ui->customPlot->graph(0)->data()->size())
          , 0);
    lastFpsKey = key;
    frameCount = 0;
  }
}
void MainWindow::serialport_read()
{
    buffer=serialPort->readLine();
    data_string+=QString::fromStdString(buffer.toStdString());
    if(data_string.contains("\n"))//doc het du lieu
    {
        // du lieu nhan dc co dang json {"name":"giatri"}\n
        // vd {"as":"88.52"}\n

        index=data_string.lastIndexOf("\"",data_string.length()-4);//tim vi tri cua chi " truoc gia tri

        data_string.remove(0,index+1);//xoa tu dau den vi tri chu "

        // xoa not "}\n
        data_string.remove(data_string.lastIndexOf("\"",data_string.length()),data_string.length());

        giatri=data_string.toDouble();//chuyen doi thanh gia tri double


        data_string="";
        index=0;
    }
}




MainWindow::~MainWindow()
{
  delete ui;
}






































