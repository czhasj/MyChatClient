#include "loginwidget.h"
#include "ui_loginwidget.h"
#include "mainwindow.h"
#include <QFile>
#include <QDebug>
#include "unit.h"

LoginWidget::LoginWidget(QWidget *parent)
    : CustomMoveWidget(parent)
    , ui(new Ui::LoginWidget)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    // QFile file(":/resource/qss/default.css");
    // file.open(QIODevice::ReadOnly);

    // //设置样式表
    // qApp->setStyleSheet(file.readAll());
    // file.close();

    myHelper::setStyle("default");

    ui->stackedWidget->setCurrentIndex(0);

    ui->lineEditUser->SetIcon(QPixmap(":/resource/common/ic_user.png"));
    ui->lineEditPasswd->SetIcon(QPixmap(":/resource/common/ic_lock.png"));

    ui->labelWinTitle->setText("用户登录");

    m_tcpsocket = new ClientSocket;

    connect(m_tcpsocket,&ClientSocket::signalStatus,this,&LoginWidget::onSignalStatus);
    connect(m_tcpsocket,&ClientSocket::signalMessage,this,&LoginWidget::onSignalMessage);

        m_tcpsocket->CheckConnected();

}

LoginWidget::~LoginWidget()
{
    delete ui;
}

void LoginWidget::on_btnWinMenu_clicked()
{
    ui->stackedWidget->setStartVal(0);
    ui->stackedWidget->setEndVal(180);
    ui->stackedWidget->animation(1);
}


void LoginWidget::on_btnCancel_clicked()
{
    ui->stackedWidget->setStartVal(0);
    ui->stackedWidget->setEndVal(-180);
    ui->stackedWidget->animation(0);
}


void LoginWidget::on_btnLogin_clicked()
{



    // QString username = ui->lineEditUser->text();
    // QString passwd = ui->lineEditPasswd->text();

    // QJsonObject json;
    // json.insert("name",username);
    // json.insert("passwd",passwd);

    // m_tcpsocket->SltSendMessage(0x11,json);
    disconnect(m_tcpsocket,&ClientSocket::signalStatus,this,&LoginWidget::onSignalStatus);
    disconnect(m_tcpsocket,&ClientSocket::signalMessage,this,&LoginWidget::onSignalMessage);
    MainWindow *mainwindow = new MainWindow;
    mainwindow->SetSocket(m_tcpsocket,ui->lineEditUser->text());
    mainwindow->show();
    this->hide();
}

void LoginWidget::onSignalMessage(const quint8 &type, const QJsonValue &dataVal)
{

}

void LoginWidget::onSignalStatus(const quint8 &state)
{
    switch (state) {
    case LoginSuccess://登录成功
    {
        qDebug()<<"登录成功";
        MainWindow *mainwindow = new MainWindow;
        mainwindow->show();
        this->hide();
        break;
    }
    case ConnectedHost://登录成功
        ui->labelWinTitle->setText("已连接服务器");
        break;
    case LoginPasswdError://用户未注册
        qDebug()<<"用户未注册";
        break;
    case LoginRepeat://用户已在线
        qDebug()<<"用户已在线";
        break;
    default:
        break;
    }
}
