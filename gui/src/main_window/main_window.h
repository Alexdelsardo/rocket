#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "connector_window/connector_window.h"

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>

class main_window : public QMainWindow{
    Q_OBJECT
    
public:
    main_window(QWidget *parent);
    ~main_window();
private:
    void create_menu();
    void custom_theme();
    connector_window *connector;
    QVBoxLayout *v_layout;
    QHBoxLayout *h_layout;
    QWidget *central_widget;

};
#endif