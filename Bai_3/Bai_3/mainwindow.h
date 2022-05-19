#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <qcustomplot.h>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
  void setupRealtimeDataDemo(QCustomPlot *customPlot);

private slots:
  void realtimeDataSlot();
  void serialport_read();


private:
  Ui::MainWindow *ui;
  QString demoName;
  QTimer dataTimer;
  QCPItemTracer *itemDemoPhaseTracer;
  double giatri=10.0;
  QByteArray buffer;
  QString data_string;
  int index=0;

};

#endif // MAINWINDOW_H
