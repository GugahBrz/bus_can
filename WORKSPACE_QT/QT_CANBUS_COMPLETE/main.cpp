#include "mainwindow.h"


#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    MainWindow_opengl w_ogl(0,800,600);
    w_ogl.show();
    w.show();

    return a.exec();
}
