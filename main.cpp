#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    // Initializes the Qt GUI Application System
    QApplication a(argc, argv);

    // Create and display our Smart Scheduler Main Window
    MainWindow w;
    w.show();

    // Start the event loop
    return a.exec();
}