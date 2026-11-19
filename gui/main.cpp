#include <QApplication>
#include "MainWindow.hpp"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("Flow Shop Permutacional");

    MainWindow ventana;
    ventana.resize(1100, 700);
    ventana.show();
    return app.exec();
}
