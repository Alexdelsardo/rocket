#ifndef CONNECTOR_WINDOW_H
#define CONNECTOR_WINDOW_H

#include <QPushButton>
#include <QSerialPort>
#include <QComboBox>
#include <QWidget>
#include <QLabel>
#include <QGridLayout>

#include <QGroupBox>

class connector_window : public QWidget{
    Q_OBJECT
    
public:
    connector_window(QWidget *parent = nullptr);
    ~connector_window();
private:
    QLabel *label_port;
    QLabel *label_baud;
    QComboBox *port_selector;
    QComboBox *baud_selector;
    QPushButton *connect_button;

    QGroupBox *connector_groupbox;
    QGroupBox *status_groupbox;
    QGridLayout *preview_layout;
    QSerialPort *serial;
    
signals:
    //void connected(); //send data signal to status display and send data stuff
private slots:
    //void try_connection(); // button press -> try connection
};
#endif