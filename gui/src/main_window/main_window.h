#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "connector_window/connector_window.h"

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QTimer>

class main_window : public QMainWindow{
    Q_OBJECT
    
public:
    main_window(QWidget *parent);
private slots:
    //connector window slots
    void try_connection();
    void read_data();
    //void handle_bytes_written();
    //status display slots
    void update_flightcomp_state();
signals:
    //connector window signals
    void connection_successful();
private:
    void create_menu();
    void custom_theme();
    void create_connector_window();
    void create_status_display();

    QTimer *timer;

    // connector window stuff
    QLabel *label_port;
    QLabel *label_baud;
    QComboBox *port_selector;
    QComboBox *baud_selector;
    QPushButton *connect_button;
    QGroupBox *connector_groupbox;
    QGridLayout *preview_layout;
    QSerialPort *serial;
    
    // status display
    QGroupBox *status_groupbox;
    QGridLayout *status_layout;
    QLabel *Flight_comp;
    QLabel *IMU;
    QLabel *S2;
    QLabel *S3;
    QLabel *S4;
    QLabel *S5;





};
#endif