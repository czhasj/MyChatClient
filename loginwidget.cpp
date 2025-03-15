#include "loginwidget.h"
#include "ui_loginwidget.h"
#include "mainwindow.h"
#include "databasemagr.h"
#include "unit.h"
#include "myapp.h"
#include "global.h"

#include <QDebug>
#include <QMessageBox>
#include <QPainter>

#include <QPropertyAnimation>
#include <QCompleter>
#include <QStringListModel>

LoginWidget::LoginWidget(QWidget *parent)
    : CustomMoveWidget(parent)
    , ui(new Ui::LoginWidget)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    m_bConnected = false;
    InitWidget();
    m_tcpSocket = new ClientSocket();
    ui->labelWinTitle->setText(tr("正在连接服务器..."));

    // connect(m_tcpSocket,&ClientSocket::signalStatus,this,&LoginWidget::onSignalStatus);
    // connect(m_tcpSocket,&ClientSocket::signalMessage,this,&LoginWidget::onSignalMessage);
    qDebug() << "MyApp::m_strHostAddr before connect:" << MyApp::m_strHostAddr;
    qDebug() << "MyApp::m_nMsgPort before connect:" << MyApp::m_nMsgPort;
    m_tcpSocket->ConnectToHost(MyApp::m_strHostAddr, MyApp::m_nMsgPort);
    // m_tcpSocket->CheckConnected();
     connect(m_tcpSocket, SIGNAL(signalStatus(quint8)), this, SLOT(SltTcpStatus(quint8)));
}

LoginWidget::~LoginWidget()
{
    delete ui;
}
void LoginWidget::SltTcpStatus(const quint8 &state)
{
    switch (state) {
    case DisConnectedHost: {
        m_bConnected = false;
        ui->labelWinTitle->setText(tr("服务器已断开"));
    }
    break;
    case ConnectedHost:
    {
        m_bConnected = true;
        ui->labelWinTitle->setText(tr("已连接服务器"));
    }
    break;
        // 登陆验证成功
    case LoginSuccess:
    {
        disconnect(m_tcpSocket, SIGNAL(signalStatus(quint8)), this, SLOT(SltTcpStatus(quint8)));

        // 登录成功后，保存当前用户
        MyApp::m_strUserName = ui->lineEditUser->text();
        MyApp::m_strPassword = ui->lineEditPasswd->text();
        MyApp::SaveConfig();

        // 显示主界面
        MainWindow *mainWindow = new MainWindow();
        if (!QFile::exists(MyApp::m_strHeadFile)) {
            myHelper::Sleep(100);
            QJsonObject jsonObj;
            jsonObj.insert("from", MyApp::m_nId);
            jsonObj.insert("id",  -2);
            jsonObj.insert("msg", myHelper::GetFileNameWithExtension(MyApp::m_strHeadFile));
            m_tcpSocket->SltSendMessage(GetFile, jsonObj);
            myHelper::Sleep(100);
        }

        // 居中显示
        myHelper::FormInCenter(mainWindow);
        mainWindow->SetSocket(m_tcpSocket, MyApp::m_strUserName);
        mainWindow->show();
        this->close();
    }
    break;
    case LoginPasswdError:
    {
        CMessageBox::Infomation(this, "登录失败，请检查用户名和密码！");
    }
    break;
    case LoginRepeat:
    {
        CMessageBox::Infomation(this, "登录失败，该账户已登录！");
    }
    break;

    case RegisterOk:
    {
        CMessageBox::Infomation(this, "该账号注册成功！请点击登录！");
    }
    break;
    case RegisterFailed:
    {
        CMessageBox::Infomation(this, "该账号已经注册！请点击登录！");
    }
    break;
    default:
        break;
    }

    // 还原初始位置，重新登录
    ui->widgetInput->setVisible(true);
    ui->labelUserHead->move(40, 115);
}
void LoginWidget::InitWidget()
{
    // 加载配置信息
    ui->lineEditUser->SetIcon(QPixmap(":/resource/common/ic_user.png"));
    ui->lineEditUser->setText(MyApp::m_strUserName);

    ui->lineEditPasswd->SetIcon(QPixmap(":/resource/common/ic_lock.png"));    // 加载之前的配置

    ui->lineEditHostAddr->setText(MyApp::m_strHostAddr);
    ui->lineEditHostMsgPort->setText(QString::number(MyApp::m_nMsgPort));
    ui->lineEditHostFilePort->setText(QString::number(MyApp::m_nFilePort));
    if (ui->checkBoxPasswd->isChecked())
    {
        ui->lineEditPasswd->setText(MyApp::m_strPassword);
    }

    QStringList valueList;
    valueList << "xiaoni" << "zhangsan" << "dengmc" << "wangwu";

    QCompleter *completer = new QCompleter(valueList, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->lineEditUser->setCompleter(completer);

    // 信号槽
    connect(ui->lineEditUser, SIGNAL(editingFinished()), this, SLOT(SltEditFinished()));
    connect(ui->lineEditUser, SIGNAL(returnPressed()), this, SLOT(on_btnLogin_clicked()));
    connect(ui->lineEditPasswd, SIGNAL(returnPressed()), this, SLOT(on_btnLogin_clicked()));

    // 默认显示第一界面
    ui->stackedWidget->setCurrentIndex(0);
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
    ui->widgetInput->setVisible(false);

    QPropertyAnimation *animation = new QPropertyAnimation(ui->labelUserHead, "pos");
    animation->setDuration(200);
    animation->setStartValue(QPoint(40, 115));
    animation->setEndValue(QPoint((this->width() - ui->labelUserHead->width()) / 2 - 20, 100));
    connect(animation, SIGNAL(finished()), this, SLOT(SltAnimationFinished()));

    animation->start();
}
void LoginWidget::SltAnimationFinished()
{
    if (!m_bConnected) {
        m_tcpSocket->ConnectToHost(MyApp::m_strHostAddr, MyApp::m_nMsgPort);
        CMessageBox::Infomation(this, "未连接服务器，请等待！");
        ui->widgetInput->setVisible(true);
        ui->labelUserHead->move(40, 115);
        return;
    }

    // 构建 Json 对象
    QJsonObject json;
    json.insert("name", ui->lineEditUser->text());
    json.insert("passwd", ui->lineEditPasswd->text());

    m_tcpSocket->SltSendMessage(Login, json);
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
