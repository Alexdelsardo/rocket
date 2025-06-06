#include "main_window.h"

#include <QMenuBar>
#include <QAction>
#include <QApplication>
#include <QPalette>
#include <QColor>
#include <QWidget>
#include <QSerialPortInfo>
#include <QDebug>
#include <QGroupBox>
#include <QDialog>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QSerialPort>
#include <QTimer>
#include <QString>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
main_window::main_window(QWidget *parent): QMainWindow(parent){


    // timer = new QTimer(this);
    // timer->start(50); //50 ms

    custom_theme();
    create_menu();
    create_connector_window();
    create_status_display();

    QWidget *central = new QWidget(this);          
    QGridLayout *layout = new QGridLayout(central); 
    layout->addWidget(connector_groupbox, 0, 1);
    layout->addWidget(status_groupbox, 0, 0);

    setCentralWidget(central);                      
    setWindowTitle("Rocket GUI");







}

void main_window::create_menu(){
    //Menu bar stuff
    QMenu *file_menu = menuBar()->addMenu(tr("&File"));
    QAction *exit_action = new QAction(tr("&Exit"), this);
    file_menu->addAction(exit_action);
    connect(exit_action, &QAction::triggered, this, &QApplication::quit);
}
void main_window::custom_theme(){

    QPalette theme;
    
    theme.setColor(QPalette::Window, QColor(0, 0, 0));
    theme.setColor(QPalette::WindowText, Qt::white);
    theme.setColor(QPalette::Base, QColor(25, 25, 25));
    theme.setColor(QPalette::AlternateBase, QColor(0, 0, 0));
    theme.setColor(QPalette::ToolTipBase, Qt::white);
    theme.setColor(QPalette::ToolTipText, Qt::white);
    theme.setColor(QPalette::Text, Qt::white);
    theme.setColor(QPalette::Button, QColor(0, 0, 0));
    theme.setColor(QPalette::ButtonText, Qt::white);
    theme.setColor(QPalette::Highlight, QColor(142, 45, 197).lighter());
    theme.setColor(QPalette::HighlightedText, Qt::black);
    this->setPalette(theme);
    this->setAutoFillBackground(true);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////                                                           Connection Window                              ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void main_window::create_connector_window(){

    connector_groupbox = new QGroupBox("Connector Window");
    port_selector = new QComboBox;
    baud_selector = new QComboBox;
    connect_button = new QPushButton("Connect");
    label_port = new QLabel("Port: ");
    label_baud = new QLabel("Baud: ");
    preview_layout = new QGridLayout();
    preview_layout->addWidget(label_port, 0, 0);
    preview_layout->addWidget(port_selector, 0, 1);
    preview_layout->addWidget(label_baud, 1, 0);
    preview_layout->addWidget(baud_selector, 1, 1);
    preview_layout->addWidget(connect_button, 2, 1, Qt::AlignRight);
  

    connector_groupbox->setLayout(preview_layout);

    
    baud_selector->addItem("-");
    baud_selector->addItem("1200");
    baud_selector->addItem("2400");
    baud_selector->addItem("4800");
    baud_selector->addItem("9600");
    baud_selector->addItem("19200");
    baud_selector->addItem("38400");

    port_selector->clear();
    port_selector->addItem("-");
    port_selector->addItem("/dev/ttyACM0");
    // sudo ln -s /dev/ttyS3 /dev/ttyUSB0   this forwards port
    //  ttyS3 maps to windows COM3
    //  Arduino Uno (COM3)
    //  Port_#0008.Hub_#0001

    // Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts()){
    //     qDebug() << "serial port: " <<port.portName();
    //     port_selector->addItem(port.portName());
    // }

    label_baud->setBuddy(baud_selector);
    label_port->setBuddy(port_selector);

    connect(connect_button, &QPushButton::clicked, this, &main_window::try_connection);

}
void main_window::try_connection(){
    qDebug() << "working";
    QString baud = baud_selector->currentText();
    serial = new QSerialPort();
    QString port = port_selector->currentText();
    qDebug() << baud;
    qDebug() << port;
    serial = new QSerialPort(this);
    connect(serial, &QSerialPort::readyRead, this, &main_window::read_data);
    //connect(serial, &QSerialPort::bytesWritten, this, &main_window::handle_bytes_written);
    serial->setPortName(port);
    if (baud == "1200") {
        serial->setBaudRate(QSerialPort::Baud1200);
    } else if (baud == "2400") {
        serial->setBaudRate(QSerialPort::Baud2400);
    } else if (baud == "4800") {
        serial->setBaudRate(QSerialPort::Baud4800);
    } else if (baud == "9600") {
        serial->setBaudRate(QSerialPort::Baud9600);
    } else if (baud == "19200") {
        serial->setBaudRate(QSerialPort::Baud19200);
    } else if (baud == "38400") {
        serial->setBaudRate(QSerialPort::Baud38400);
    } else {
        serial->setBaudRate(QSerialPort::Baud9600); // default case :3
    }
    if (serial->open(QIODevice::ReadOnly)){
        emit connection_successful();
    }else{
        qDebug() << "Failed to open serial port:" << serial->errorString();
    }

   
}
void main_window::read_data(){

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////                                                           Status Display                              ///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void main_window::create_status_display(){

    status_groupbox = new QGroupBox("Status");
    status_layout = new QGridLayout();

    QLabel *l1 = new QLabel("Flight Computer: ");
    QLabel *l2 = new QLabel("IMU: ");
    QLabel *l3 = new QLabel("Sensor2: ");
    QLabel *l4 = new QLabel("Sensor3: ");
    QLabel *l5 = new QLabel("Sensor4: ");
    QLabel *l6 = new QLabel("Sensor5: ");

    status_layout->addWidget(l1, 0, 0);
    status_layout->addWidget(l2, 1, 0);
    status_layout->addWidget(l3, 2, 0);
    status_layout->addWidget(l4, 3, 0);
    status_layout->addWidget(l5, 4, 0);
    status_layout->addWidget(l6, 5, 0);
    Flight_comp = new QLabel("Not Connected");
    IMU = new QLabel("Not Active");
    Flight_comp->setStyleSheet("QLabel { color : red; }");
    IMU->setStyleSheet("QLabel { color : red; }");

    connect(this, &main_window::connection_successful, this, &main_window::update_flightcomp_state);

    status_layout->addWidget(Flight_comp, 0, 1);
    status_layout->addWidget(IMU, 1, 1);
    status_groupbox->setLayout(status_layout);
}

void main_window::update_flightcomp_state() {
    Flight_comp->setText("Connected");
    Flight_comp->setStyleSheet("QLabel { color : green; }");

    IMU->setText("Active");
    IMU->setStyleSheet("QLabel { color : green; }");


}

