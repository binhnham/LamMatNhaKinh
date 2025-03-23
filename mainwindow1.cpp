#include "mainwindow1.h"
#include "ui_mainwindow1.h"
#include <QVBoxLayout>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>

QFile dataFile;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), serial(new QSerialPort(this)), timeCounter(0) {
    ui->setupUi(this);

    ui->motorStatusLabel->setText("Motor OFF (A)");
    ui->motorStatusLabel2->setText("Motor OFF (M)");

    // Setup plot
    ui->TempTime->addGraph();
    ui->TempTime->graph(0)->setPen(QPen(Qt::blue));
    ui->TempTime->xAxis->setLabel("Time (s)");
    ui->TempTime->yAxis->setLabel("Temperature (°C)");

    // Setup serial port
    connect(serial, &QSerialPort::readyRead, this, &MainWindow::readSerialData);

    // Setup motor button
    connect(ui->motorON, &QPushButton::clicked, this, &MainWindow::turnMotorOn);
    connect(ui->motorOFF, &QPushButton::clicked, this, &MainWindow::turnMotorOff);

    // Setup start/stop buttons
    connect(ui->connBtn, &QPushButton::clicked, this, &MainWindow::onStartReading);
    connect(ui->discBtn, &QPushButton::clicked, this, &MainWindow::onStopReading);
}

MainWindow::~MainWindow() {
    if (serial->isOpen()) {
        serial->close();
    }
    delete ui;
}

void MainWindow::onStartReading() {
    serial->setPortName("COM11"); // Adjust for your system
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if (serial->open(QIODevice::ReadWrite)) {
    connect(serial, &QSerialPort::readyRead, this, &MainWindow::readSerialData);
        ui->statusLabel->setText("Reading data...");

        // Mở tệp CSV
                QString fileName = QFileDialog::getSaveFileName(this, "Save Data", "", "CSV Files (*.csv)");
                if (!fileName.isEmpty()) {
                    dataFile.setFileName(fileName);
                    if (dataFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
                        QTextStream out(&dataFile);
                        out << "Time (s),Temperature (°C)\n"; // Ghi tiêu đề cột
            }
        }
    } else {
    qDebug() << "Failed to open serial port: " << serial->errorString();
        ui->statusLabel->setText("Failed to open serial port.");
    }
}

void MainWindow::onStopReading() {
    if (serial->isOpen()) {disconnect(serial, &QSerialPort::readyRead, this, &MainWindow::readSerialData);
        serial->close();
        ui->statusLabel->setText("Stopped reading data.");

        // Đóng tệp CSV
            if (dataFile.isOpen()) {
                dataFile.close();
            }
    }
}
void MainWindow::readSerialData() {
    QByteArray data = serial->readAll(); // Đọc toàn bộ dữ liệu từ Serial
    QString temperature = QString(data).trimmed(); // Loại bỏ khoảng trắng và ký tự không cần thiết

    /*if (temperature.contains("Motor ON")) {
    ui->motorStatusLabel->setText("Motor ON (M)");
    } else if (temperature.contains("Motor OFF")) {
    ui->motorStatusLabel->setText("Motor OFF (M)");
      }*/

    // Tìm giá trị số thực trong chuỗi (nếu có)
    bool isNumber = false;
    double tempValue = temperature.toDouble(&isNumber);

    if (isNumber) {
        // Nếu dữ liệu hợp lệ, hiển thị trên giao diện
        ui->temperatureLabel->setText(QString::number(tempValue, 'f', 2) + "°C");
        ui->statusLabel->setText("Data temperature received.");

        // Cập nhật trạng thái động cơ dựa trên nhiệt độ
                if (tempValue >= 27.0) {
                    ui->motorStatusLabel->setText("Motor ON (A)");
                    ui->Alert->setText("Warning");
                } else {
                    ui->motorStatusLabel->setText("Motor OFF (A)");
                    ui->Alert->setText("");
                }
    }

    bool ok;
    float temp = temperature.toFloat(&ok);
    if (ok) {
        updatePlot(temp);
    }
}

void MainWindow::updatePlot(float temperature) {
    ui->TempTime->graph(0)->addData(timeCounter++, temperature);
    ui->TempTime->graph(0)->rescaleValueAxis();
    ui->TempTime->xAxis->setRange(qMax(0, timeCounter - 10), timeCounter);
    //ui->TempTime->yAxis->setRange(20,30);
    ui->TempTime->replot();

    // Ghi dữ liệu vào tệp CSV
        if (dataFile.isOpen()) {
            QTextStream out(&dataFile);
            out << timeCounter << "," << temperature << "\n";
        }
}

void MainWindow::turnMotorOn() {
    if (serial->isOpen()) {
        serial->write("ON"); // Gửi lệnh bật động cơ
        ui->motorStatusLabel2->setText("Motor ON (M)");
}
}
void MainWindow::turnMotorOff() {
    if (serial->isOpen()) {
        serial->write("OFF"); // Gửi lệnh tắt động cơ
        ui->motorStatusLabel2->setText("Motor OFF (M)");
}
}
