#ifndef CONNECTOR_WINDOW_H
#define CONNECTOR_WINDOW_H

#include <QPushButton>
#include <QSerialPort>
#include <QComboBox>
#include <QWidget>
#include <QLabel>


class connector_window : public QWidget{
    Q_OBJECT
    
public:
    connector_window(QWidget *parent);
    ~connector_window();
private:
    QLabel *label;
    QSerialPort *serial;
    QComboBox *port_selector;
    QComboBox *baud_selector;
    QPushButton *connect_button;
    void populate_ports(); // show all serial ports
signals:
    //void connected(); //send data signal to status display and send data stuff
private slots:
    //void try_connection();
};
#endif