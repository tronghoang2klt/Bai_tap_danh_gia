#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void addPoint(double x,double y);
    void plot();

private slots:

    void serialport_read();
private:
    Ui::MainWindow *ui;
    QVector<double> qv_x,qv_y;
    double time=0;
    QChar charBuffer[6];
    double giatri=10.0;
    int index=0;
    QByteArray buffer;
    QString data_string;
};
#endif // MAINWINDOW_H
