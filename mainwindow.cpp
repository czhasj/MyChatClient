#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMenu>
#include "chatstackedwidget.h"
#include "qqcell.h"
#include "databasemagr.h"
#include "global.h"
#include <QJsonObject>
MainWindow::MainWindow(QWidget *parent)
    : CustomMoveWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);
    // ui->widget_5->raise();

    m_btnGroup = new QButtonGroup(this);
    m_btnGroup->addButton(ui->btnFrind,0);
    m_btnGroup->addButton(ui->btnGroup,1);
    m_btnGroup->addButton(ui->btnConversation,2);
    m_btnGroup->addButton(ui->btnApplay,3);
    // connect(m_btnGroup,&QButtonGroup::idClicked,this,&MainWindow::stlMainPageChanged);
    ui->GCStackedWidget->setCurrentIndex(0);
    InitQQListMenu();
    InitSysTrayIcon();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SetSocket(ClientSocket *tcpSocket, const QString &name)
{
    if(tcpSocket != NULL){
        m_tcpsocket = tcpSocket;
        connect(m_tcpsocket,&ClientSocket::signalStatus,this,&MainWindow::SltTcpStatus);
        connect(m_tcpsocket,&ClientSocket::signalMessage,this,&MainWindow::SltTcpReply);
    }
}

void MainWindow::stlMainPageChanged(int index)
{
    static int s_nIndex = 0;//记录当前选中的的页
    if(s_nIndex == index){
        return;
    }

    ui->GCStackedWidget->setLength(ui->GCStackedWidget->width(),
    index > s_nIndex ? ChatStackedWidget::LeftToRight : ChatStackedWidget::RightToLeft);
    ui->GCStackedWidget->start(index);
    s_nIndex = index;
}

void MainWindow::SltTrayIcoClicked(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::DoubleClick:{
        if(!this->isVisible()){
            this->show();
        }
    }
        break;
    default:
        break;
    }
}

void MainWindow::SltTrayIconMenuClicked(QAction* action)
{
    if("退出" == action->text()){
        this->hide();
        QTimer::singleShot(500,this,SLOT(SltQuitApp()));
    }else if("显示主面板" == action->text()){
        this->show();
    }else if(!QString::compare("我在线上",action->text())){
        m_tcpsocket->CheckConnected();
    }else if(!QString::compare("离线",action->text())){
        m_tcpsocket->ColseConnected();
    }
}

void MainWindow::SltQuitApp()
{
    delete ui;
    qApp->quit();
}

void MainWindow::SltTcpReply(const quint8 &type, const QJsonValue &dataVal)
{
    switch (type) {
    case AddFriend:
        PraseAddFriendReply(dataVal);
        break;
    case AddFriendRequist:
        PraseAddFriendRequistReply(dataVal);
        break;
    default:
        break;
    }
}
void MainWindow::PraseAddFriendReply(const QJsonValue &dataVal)
{
    if(dataVal.isObject()){
        QJsonObject datajson = dataVal.toObject();
        int id = datajson.value("id").toInt();
        QString name = datajson.value("name").toString();
        int status = datajson.value("status").toInt();
        QString head = datajson.value("head").toString();

        QQCell *cell = new QQCell;
        cell->groupName = QString("我的好友");
        cell->iconPath = QString(":/resource/head/%1").arg(head);
        cell->type = QQCellType_Child;
        cell->name = name;
        cell->subTitle = QString("当前用户状态：在线");
        cell->id = id;
        cell->status = status;

        ui->frindListWidget->insertQQCell(cell);
    }
}

void MainWindow::PraseAddFriendRequistReply(const QJsonValue &dataVal)
{
    if(dataVal.isObject()){
        QJsonObject datajson = dataVal.toObject();
        int id = datajson.value("id").toInt();
        QString name = datajson.value("name").toString();
        QString head = datajson.value("head").toString();
        QString msg = datajson.value("msg").toString();

        if(!DataBaseMagr::Instance()->isMyFriend(MyApp::m_nId,name)){
            QQCell *cell = new QQCell;
            cell->groupName = QString("我的好友");
            cell->iconPath = QString(":/resource/head/%1").arg(head);
            cell->type = QQCellType_Child;
            cell->name = name;
            cell->subTitle = QString("当前用户状态：在线");
            cell->id = id;
            cell->status = OnLine;

            ui->frindListWidget->insertQQCell(cell);
            DataBaseMagr::Instance()->AddFriend(cell->id,MyApp::m_nId,name);
        }

    }
}
void MainWindow::SltTcpStatus(const quint8 &state)
{

}

void MainWindow::InitSysTrayIcon()
{
    systemTrayIcon = new QSystemTrayIcon(this);
    systemTrayIcon->setIcon(QIcon(":/resource/background/app.png"));

    QMenu *m_trayMenu = new QMenu(this);
    m_trayMenu->addAction("我在线上");
    m_trayMenu->addAction("离线");
    m_trayMenu->addSeparator();
    m_trayMenu->addAction("显示主面板");
    m_trayMenu->addSeparator();
    m_trayMenu->addAction("退出");

    systemTrayIcon->setContextMenu(m_trayMenu);
    systemTrayIcon->show();

    connect(systemTrayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(SltTrayIcoClicked(QSystemTrayIcon::ActivationReason)));
    connect(m_trayMenu,SIGNAL(triggered(QAction*)),this,SLOT(SltTrayIconMenuClicked(QAction*)));
}



void MainWindow::on_btnWinMin_clicked()
{
    this->hide();
}


void MainWindow::on_btnWinClose_clicked()
{
    // qApp->quit();
    SltQuitApp();
}
/**
 * @brief MainWindow::onAddFriendMenuDidSelected
 * 添加好友
 * @param action
 */
void MainWindow::onAddFriendMenuDidSelected(QAction *action)
{
    if (!action->text().compare(tr("添加好友")))
    {
        QString text = CInputDialog::GetInputText(this, "milo");

        if (!text.isEmpty()) {
            // 首先判断该用户是否已经是我的好友了
            if (DataBaseMagr::Instance()->isMyFriend(MyApp::m_nId, text)) {
                CMessageBox::Infomation(this, "该用户已经是你的好友了！");
                return;
            }

            // 构建 Json 对象
            QJsonObject json;
            json.insert("id", m_tcpsocket->GetUserId());
            json.insert("name", text);

            m_tcpsocket->SltSendMessage(AddFriend, json);
        }
    }
    else if (!action->text().compare(tr("刷新")))
    {
        // 上线的时候获取当前好友的状态
        QJsonArray friendArr = DataBaseMagr::Instance()->GetMyFriend(MyApp::m_nId);

        // 组织Jsonarror
        m_tcpsocket->SltSendMessage(RefreshFriends, friendArr);
    }
    else if (!action->text().compare(tr("删除该组")))
    {
        qDebug() << "delete group";
    }
}

void MainWindow::onGroupPopMenuDidSelected(QAction *action)
{
    if (!action->text().compare(tr("创建讨论组")))
    {
        QString text = CInputDialog::GetInputText(this, "我的朋友们");
        if (!text.isEmpty()) {

            // 构建 Json 对象
            QJsonObject json;
            json.insert("id", m_tcpsocket->GetUserId());
            json.insert("name", text);

            m_tcpsocket->SltSendMessage(CreateGroup, json);
        }
    }
    else if (!action->text().compare(tr("加入讨论组")))
    {
        QString text = CInputDialog::GetInputText(this, "我的朋友们");
        if (!text.isEmpty()) {
            // 首先判断是否已经添加该群组了
            // if (DataBaseMagr::Instance()->isInGroup(text)) {
            //     CMessageBox::Infomation(this, "你已经添加该群组了！");
            //     return;
            // }

            // 构建 Json 对象
            QJsonObject json;
            json.insert("id", MyApp::m_nId);
            json.insert("name", text);

            m_tcpsocket->SltSendMessage(AddGroup, json);
        }
    }
    else if (!action->text().compare(tr("删除该组")))
    {
        qDebug() << "delete group";
    }
}

void MainWindow::onChildPopMenuDidSelected(QAction *action)
{
    QQCell *cell = ui->frindListWidget->GetRightClickedCell();
    if (NULL == cell) return;

    if (!action->text().compare(tr("发送即时消息")))
    {
        qDebug() << "send message" << cell->name;
        // 打开对话框
        //SltFriendsClicked(cell);
    }
    else if (!action->text().compare(tr("移动至黑名单")))
    {
        qDebug() << "balck friend list" << cell->name;
    }
    else if (!action->text().compare(tr("删除联系人")))
    {
        qDebug() << "delete my friend" << cell->name;
        ui->frindListWidget->removeQQCell(cell);
        // 更新数据库
        //bool bOk = DataBaseMagr::Instance()->DeleteMyFriend(cell->id, MyApp::m_nId);
        //CMessageBox::Infomation(this, bOk ? tr("用户删除成功！") : tr("用户删除失败！"));
    }
}

/**
 * @brief MainWindow::InitQQListMenu
 * 添加好友列表菜单
 */
void MainWindow::InitQQListMenu()
{
    //设置菜单
    //设置组菜单
    QMenu *addFriend = new QMenu(this);
    addFriend->addAction(tr("添加好友"));
    addFriend->addAction(tr("刷新"));
    addFriend->addSeparator();
    addFriend->addAction(tr("删除该组"));
    connect(addFriend, SIGNAL(triggered(QAction*)), this, SLOT(onAddFriendMenuDidSelected(QAction*)));
    ui->frindListWidget->setGroupPopMenu(addFriend);

    // 设置子菜单
    QMenu *childMenu = new QMenu(this);
    childMenu->addAction(tr("发送即时消息"));
    childMenu->addSeparator();
    childMenu->addAction("移动至黑名单");
    childMenu->addAction("删除联系人");

    QMenu *groupListMenu = new QMenu(tr("移动联系人至"));
    childMenu->addMenu(groupListMenu);

    // childMenu->addAction(tr("创建讨论组"));
    connect(childMenu, SIGNAL(triggered(QAction*)), this, SLOT(onChildPopMenuDidSelected(QAction*)));
    ui->frindListWidget->setChildPopMenu(childMenu);

    //添加默认项
    //好友列表
    QQCell *myFriend = new QQCell;
    myFriend->groupName = QString(tr("我的好友"));
    myFriend->isOpen = false;
    myFriend->type = QQCellType_Group;
    myFriend->name = QString(tr("我的好友"));
    myFriend->subTitle = QString("(0/0)");
    ui->frindListWidget->insertQQCell(myFriend);

    QQCell *blacklist = new QQCell;
    blacklist->groupName = QString(tr("黑名单"));
    blacklist->isOpen = false;
    blacklist->type = QQCellType_Group;
    blacklist->name = QString(tr("黑名单"));
    blacklist->subTitle = QString("(0/0)");
    ui->frindListWidget->insertQQCell(blacklist);

    connect(ui->frindListWidget, SIGNAL(onChildDidDoubleClicked(QQCell*)), this, SLOT(SltFriendsClicked(QQCell*)));

    //组列表
    QMenu *myGroupMenu = new QMenu(this);
    myGroupMenu->addAction(tr("创建讨论组"));
    myGroupMenu->addAction(tr("加入讨论组"));
    myGroupMenu->addAction(tr("删除该组"));
    myGroupMenu->addSeparator();
    connect(myGroupMenu, SIGNAL(triggered(QAction*)), this, SLOT(onGroupPopMenuDidSelected(QAction*)));
    ui->groupListWidget->setGroupPopMenu(myGroupMenu);
    //设置子菜单
    QMenu *groupChildMen = new QMenu(this);
    groupChildMen->addAction(tr("发送即时消息"));
    ui->groupListWidget->setChildPopMenu(groupChildMen);

    //添加默认项
    QQCell *groupCell = new QQCell;
    groupCell->groupName = QString(tr("我的群组"));
    groupCell->isOpen = false;
    groupCell->type = QQCellType_GroupEx;
    groupCell->name = QString(tr("讨论组"));
    groupCell->subTitle = QString("(0/0)");
    ui->groupListWidget->insertQQCell(groupCell);

    connect(ui->groupListWidget, SIGNAL(onChildDidDoubleClicked(QQCell*)), this, SLOT(SltGroupsClicked(QQCell*)));
}
