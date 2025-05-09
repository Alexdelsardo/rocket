#include "main_window.h"

#include <QMenuBar>
#include <QAction>
#include <QApplication>
#include <QPalette>
#include <QColor>
#include <QWidget>
main_window::main_window(QWidget *parent): QMainWindow(parent){
    setFixedSize(1600, 800);
    setWindowTitle("Rocket GUI");


    custom_theme();
    create_menu();
    // main layout
    v_layout = new QVBoxLayout;
    h_layout = new QHBoxLayout;
    central_widget = new QWidget(this);
    central_widget->setLayout(v_layout);
    setCentralWidget(central_widget);
    central_widget->setStyleSheet("border: 2px solid white;");

    //add connector window
    connector = new connector_window(this);

    h_layout->addWidget(connector, 0, Qt::AlignTop | Qt::AlignRight );
    v_layout->addLayout(h_layout);






















}

main_window::~main_window(){


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