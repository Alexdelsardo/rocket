#include "connector_window.h"


#include <QSerialPortInfo>
#include <QVBoxLayout>
#include <QSerialPort>
#include <QWidget>
#include <QDebug>
#include <QFrame>

#include <QGroupBox>

#include <QApplication>
#include <QDialog>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
connector_window::connector_window(QWidget *parent): QWidget(parent){

    connector_groupbox = new QGroupBox(tr("Connector Window"));

    port_selector = new QComboBox;
    baud_selector = new QComboBox;
    connect_button = new QPushButton;
    label_port = new QLabel(tr("Port: "));
    label_baud = new QLabel(tr("Baud: "));
    preview_layout = new QGridLayout();
    // preview_layout->addWidget(label_port, 0, 0);
    // preview_layout->addWidget(port_selector, 0, 1);
    // preview_layout->addWidget(label_baud, 1, 0);
    // preview_layout->addWidget(baud_selector, 1, 1);
    // preview_layout->addWidget(connect_button, 2, 1, Qt::AlignRight);
  

    //connector_groupbox->setLayout(preview_layout);

    

    baud_selector->addItem(tr("4800"));
    baud_selector->addItem(tr("9600"));
    baud_selector->addItem(tr("19200"));
    baud_selector->addItem(tr("38400"));
    baud_selector->addItem(tr("57600"));
    baud_selector->addItem(tr("115200"));

    port_selector->clear();
    Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts()){
        qDebug() << "serial port: " << QSerialPortInfo::availablePorts().count();
        port_selector->addItem(port.portName());
    }

    label_baud->setBuddy(baud_selector);
    label_port->setBuddy(port_selector);

}

connector_window::~connector_window(){


}

