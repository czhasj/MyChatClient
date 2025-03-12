#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "customwidget.h"
#include "clientsocket.h"
#include <QButtonGroup>
#include <QSystemTrayIcon>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public CustomMoveWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void SetSocket(ClientSocket *tcpSocket,const QString &name);

    void InitQQListMenu();
private slots:
    void stlMainPageChanged(int index);

    void SltTrayIcoClicked(QSystemTrayIcon::ActivationReason reason);
    void SltTrayIconMenuClicked(QAction* action);
    void SltQuitApp();

    // 右键菜单
    void onAddFriendMenuDidSelected(QAction *action);
    void onGroupPopMenuDidSelected(QAction *action);
    void onChildPopMenuDidSelected(QAction *action);

    void SltTcpReply(const quint8 &type, const QJsonValue &dataVal);
    void SltTcpStatus(const quint8 &state);

    void on_btnWinMin_clicked();

    void on_btnWinClose_clicked();

private:
    Ui::MainWindow *ui;

    QButtonGroup *m_btnGroup;

    QSystemTrayIcon *systemTrayIcon;

    ClientSocket *m_tcpsocket;

    void InitSysTrayIcon();

    void PraseAddFriendReply(const QJsonValue &dataVal);
    void PraseAddFriendRequistReply(const QJsonValue &dataVal);
};
#endif // MAINWINDOW_H
