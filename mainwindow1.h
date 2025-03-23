#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onStartReading(); // Khởi động giao tiếp Serial
    void onStopReading(); // Dừng giao tiếp Serial
    void readSerialData(); // Đọc dữ liệu từ Serial
    void turnMotorOn();
    void turnMotorOff();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial; // Cổng COM
    QPushButton *motorOnButton;
    QPushButton *motorOffButton;
    int timeCounter;
    void updatePlot(float temperature);
};

#endif // MAINWINDOW_H
