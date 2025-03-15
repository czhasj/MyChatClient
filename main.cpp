#include "mainwindow.h"
#include "loginwidget.h"
#include <QApplication>
#include "global.h"
#include "myapp.h"
#include "databasemagr.h"
#include <QHostInfo>
#include <QTextCodec>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // MainWindow w;
    a.setQuitOnLastWindowClosed(false);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));

    MyApp::InitApp(a.applicationDirPath());
    myHelper::setStyle("default");
    DataBaseMagr::Instance()->OpenUserDb(MyApp::m_strDatabasePath + "user.db");
    LoginWidget w;
    myHelper::FormInCenter(&w);
    w.show();
    return a.exec();
}
