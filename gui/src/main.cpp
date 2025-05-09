#include "main_window/main_window.h"

#include <QApplication>


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    main_window window(nullptr);   // Create the main window
    window.show();        // Show the window

    return app.exec();    // Enter the Qt event loop
}