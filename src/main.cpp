#include <QApplication>
#include "mainwindow.h"
#include <QDebug>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    qDebug() << "Creating MainWindow...";
    MainWindow window;
    
    qDebug() << "Setting window properties...";
    window.setWindowTitle("Webcam Viewer");
    window.setGeometry(100, 100, 1024, 768);
    
    qDebug() << "Showing window...";
    window.show();
    window.raise();
    window.activateWindow();
    
    qDebug() << "Window shown. Starting event loop...";
    return app.exec();
}
