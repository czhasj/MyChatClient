#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include "customwidget.h"
#include "clientsocket.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <qjsonvalue.h>
#include "global.h"
namespace Ui {
class LoginWidget;
}

class LoginWidget : public CustomMoveWidget
{
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = nullptr);
    ~LoginWidget();

private slots:
    void on_btnWinMenu_clicked();

    void on_btnCancel_clicked();

    void on_btnLogin_clicked();
    void SltAnimationFinished();
    void SltTcpStatus(const quint8 &state);

    void onSignalMessage(const quint8 &type, const QJsonValue &dataVal);
    void onSignalStatus(const quint8 &state);
// signals:
//     void signalStatus(const quint8 &state);
private:
    Ui::LoginWidget *ui;
    bool m_bConnected;
    ClientSocket *m_tcpSocket;
    void InitWidget();
};

#endif // LOGINWIDGET_H
