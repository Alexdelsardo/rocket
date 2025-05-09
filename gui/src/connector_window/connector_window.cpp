#include "connector_window.h"


#include <QSerialPortInfo>
#include <QVBoxLayout>
#include <QSerialPort>
#include <QWidget>
#include <QDebug>
connector_window::connector_window(QWidget *parent): QWidget(parent){
    setFixedSize(300, 150);
    setPalette(parent->palette());
    setAutoFillBackground(true);
    
    label = new QLabel("Collector Widget", this);
    port_selector = new QComboBox;
    baud_selector = new QComboBox;
    connect_button = new QPushButton("Connect");

    baud_selector->addItems({"9600", "115200", "921600"});
    port_selector->setFixedWidth(200);  
    baud_selector->setFixedWidth(200);
    
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    layout->addWidget(port_selector);
    layout->addWidget(baud_selector);
    layout->addWidget(connect_button);
    
    layout->setAlignment(port_selector, Qt::AlignRight);
    layout->setAlignment(baud_selector, Qt::AlignRight);
    layout->setAlignment(connect_button, Qt::AlignRight);   
    setLayout(layout); 
    

    populate_ports();

}
connector_window::~connector_window(){


}

void connector_window::populate_ports(){ // will not show ports in wsl, however this is probably due to using wsl on windows :3
    port_selector->clear();
    Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts()){
        qDebug() << "serial port: " << QSerialPortInfo::availablePorts().count();
        port_selector->addItem(port.portName());
    }
}