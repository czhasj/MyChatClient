#include "mainwindow.h"
#include "loginwidget.h"
#include <QApplication>
#include "global.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // MainWindow w;
    LoginWidget w;
    myHelper::FormInCenter(&w);
    w.show();
    return a.exec();
}
