#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    QMainWindow window;
    window.setWindowTitle("Test Window");
    window.setGeometry(100, 100, 400, 300);
    
    QWidget *central = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(central);
    QLabel *label = new QLabel("If you see this, Qt GUI is working!");
    layout->addWidget(label);
    
    window.setCentralWidget(central);
    window.show();
    
    return app.exec();
}
