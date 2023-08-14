#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "friendcon.h"
#include "groupcon.h"
#include "admincon.h"
#include "friendinfo.h"
#include "admininfo.h"
#include "public.h"
#include "qsocket.h"
#include "addfrienddia.h"
#include "addgroupdia.h"
#include "creategroupdia.h"
#include "departcon.h"

#include <QDebug>
#include <QTimer>
#include <QThread>
#include <QPushButton>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::SplashScreen|Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);

    ui->stackedWidget->setCurrentIndex(0);

    //初始化系统托盘
    systemtrayicon = new QSystemTrayIcon(this);
    QIcon icon = QIcon(":/owl");
    //添加图标
    systemtrayicon->setIcon(icon);
    //当鼠标悬浮，显示文字
    systemtrayicon->setToolTip(QObject::trUtf8("IMCPP"));
    //显示图标
    systemtrayicon->show();
    menu = new QMenu(this);
    m_pShowAction = new QAction("打开主界面");
    m_pCloseAction = new QAction("退出");
    menu->addAction(m_pShowAction);
    menu->addSeparator();
    menu->addAction(m_pCloseAction);
    systemtrayicon->setContextMenu(menu);
    connect(m_pShowAction,SIGNAL(triggered(bool)),this,SLOT(showwidget()));
    connect(m_pCloseAction,SIGNAL(triggered(bool)),this,SLOT(closewidget()));

    FriendCon::getInstance()->setFriendsList(ui->friendList);
    FriendCon::getInstance()->setMessageBrowser(ui->friendTextBro);
    FriendCon::getInstance()->setFriendName(ui->friendNameLabel);
    FriendCon::getInstance()->setSendTextEdit(ui->friendTextEdit);
    FriendCon::getInstance()->setSendPushBtn(ui->friendSendBtn);

    GroupCon::getInstance()->setGroupsList(ui->groupsListWidget);
    GroupCon::getInstance()->setGroupUsersList(ui->groupUsersListWidget);
    GroupCon::getInstance()->setGroupTextBrow(ui->groupChatTextBrow);
    GroupCon::getInstance()->setGroupNameLabel(ui->groupNameLabel);
    GroupCon::getInstance()->setGroupSendTextEdit(ui->groupSendTextEdit);
    GroupCon::getInstance()->setGroupDescBrow(ui->groupDescTextBrow);

    DepartCon::getInstance()->setDepartUsersList(ui->departUserListWidget);
    DepartCon::getInstance()->setDepartTextBrow(ui->departChatTextBrow);
    DepartCon::getInstance()->setDepartSendTextEdit(ui->departSendTextEdit);
    DepartCon::getInstance()->setDepartNameLabel(ui->departNameLabel);
    DepartCon::getInstance()->setDepartDescTextBrow(ui->departNoticeTextBrow);

    connect(AdminCon::getInstance(),&AdminCon::updateAdmin,this,&MainWindow::updateAdminData);
    connect(FriendCon::getInstance(),&FriendCon::showNewFriends,this,&MainWindow::showFriendsList);
    connect(GroupCon::getInstance(),&GroupCon::showNewGroups,this,&MainWindow::showGroupsList);
    connect(GroupCon::getInstance(),&GroupCon::showNewUsers,this, &MainWindow::showGroupUsers);
    connect(DepartCon::getInstance(),&DepartCon::showNewDepartUsers,this,&MainWindow::showDepartUsers);

    connect(ui->friendList,&QListWidget::itemClicked,FriendCon::getInstance(),&FriendCon::onFriendSelected);
    connect(ui->friendSendBtn,&QPushButton::clicked,FriendCon::getInstance(),&FriendCon::onFriendSendBtn);

    connect(ui->groupsListWidget,&QListWidget::itemClicked,GroupCon::getInstance(),&GroupCon::onGroupSelect);
    connect(ui->groupUsersListWidget,&QListWidget::itemClicked,GroupCon::getInstance(),&GroupCon::onGroupUserSelect);
    connect(ui->groupSendBtn,&QPushButton::clicked,GroupCon::getInstance(),&GroupCon::onSendBtn);

    connect(ui->departUserListWidget,&QListWidget::itemClicked,DepartCon::getInstance(),&DepartCon::onDepartUserSelect);
    connect(ui->departSendBtn,&QPushButton::clicked,DepartCon::getInstance(),&DepartCon::onSendBtn);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    isPressedWidget = true; // 当前鼠标按下的即是QWidget而非界面上布局的其它控件
    last = event->globalPos();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (isPressedWidget)
    {
        int dx = event->globalX() - last.x();
        int dy = event->globalY() - last.y();
        last = event->globalPos();
        move(x()+dx, y()+dy);
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    int dx = event->globalX() - last.x();
    int dy = event->globalY() - last.y();
    move(x()+dx, y()+dy);
    isPressedWidget = false; // 鼠标松开时，置为false
}

void MainWindow::updateAdminData()
{
    ui->adminNameLabel->setText(AdminCon::getInstance()->getAdminName());
    ui->adminPhoneLabel->setText(AdminCon::getInstance()->getAdminPhone());
    ui->adminEmailLabel->setText(AdminCon::getInstance()->getAdminEmail());
    ui->adminDepartLabel->setText(AdminCon::getInstance()->getAdminDepartName());
    ui->adminDescBrow->setText(AdminCon::getInstance()->getAdminDesc());
}

void MainWindow::showFriendsList(const QList<QString>&friends)
{
    //qDebug()<<"text";
    ui->friendList->clear();
    QFont font = ui->friendList->font();
    font.setPointSize(16);
    ui->friendList->setFont(font);
    ui->friendList->addItems(friends);
}

void MainWindow::showGroupsList(const QList<QString> &groups)
{
    ui->groupsListWidget->clear();
    QFont font = ui->groupsListWidget->font();
    font.setPointSize(16);
    ui->groupsListWidget->setFont(font);
    ui->groupsListWidget->addItems(groups);
}

void MainWindow::showGroupUsers(const QList<QString> &users)
{
    ui->groupUsersListWidget->clear();
    QFont font = ui->groupUsersListWidget->font();
    font.setPointSize(16);
    ui->groupUsersListWidget->setFont(font);
    ui->groupUsersListWidget->addItems(users);
}

void MainWindow::showDepartUsers(const QList<QString> &users)
{
    ui->departUserListWidget->clear();
    QFont font = ui->departUserListWidget->font();
    font.setPointSize(16);
    ui->departUserListWidget->setFont(font);
    ui->departUserListWidget->addItems(users);
}

void MainWindow::on_CloseBtn_clicked()
{
    this->close();
}


void MainWindow::on_MinBtn_clicked()
{
    this->hide();
}


void MainWindow::on_MaxBtn_clicked()
{
    if(windowState()==Qt::WindowMaximized)
    {
        setWindowState(Qt::WindowNoState);
    }
    else
    {
        setWindowState(Qt::WindowMaximized);
    }
}

void MainWindow::on_InfoBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    ui->adminNameLabel->setText(AdminCon::getInstance()->getAdminName());
    ui->adminEmailLabel->setText(AdminCon::getInstance()->getAdminEmail());
    ui->adminPhoneLabel->setText(AdminCon::getInstance()->getAdminPhone());
    ui->adminDescBrow->setText(AdminCon::getInstance()->getAdminDesc());
}


void MainWindow::on_FriendBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::on_IconBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_DepartBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}


void MainWindow::on_GroupBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::showwidget()
{
    this->show();
}

void MainWindow::closewidget()
{
    QApplication::quit();
}

void MainWindow::on_friendMenuBtn_clicked()
{
    friendInfo UI(this);
    UI.setModal(true);
    UI.exec();
}

void MainWindow::on_reviseBtn_clicked()
{
    adminInfo UI(this);
    UI.setModal(true);
    UI.exec();
}


void MainWindow::on_MenuBtn_clicked()
{

}


void MainWindow::on_friendFlushBtn_clicked()
{
    QJsonObject data;
    data["msgid"] = QUERY_FRIEND_MSG;
    data["userName"] = AdminCon::getInstance()->getAdminName();

    QJsonDocument doc(data);
    QByteArray json = doc.toJson();
    QSocket::getInstance().sendData(json);
}


void MainWindow::on_friendAddBtn_clicked()
{
    addFriendDia UI(this);
    UI.setModal(this);
    UI.exec();
}

void MainWindow::on_historyChatBtn_clicked()
{
    QJsonObject data;
    data["msgid"] = QUERY_CHAT_MSG;
    data["recvName"] = AdminCon::getInstance()->getAdminName();
    data["sendName"] = ui->friendNameLabel->text();

    QJsonDocument doc(data);
    QByteArray json = doc.toJson();
    QSocket::getInstance().sendData(json);
}


void MainWindow::on_groupMenuBtn_clicked()
{
    QString groupDesc;
    groupDesc+="群组姓名："+GroupCon::getInstance()->_groupmodel.getCurrentGroup().groupName+"\n";
    groupDesc+="群组描述："+GroupCon::getInstance()->_groupmodel.getCurrentGroup().groupDesc+"\n";
    GroupCon::getInstance()->getGroupDescBrow()->setText(groupDesc);
}

void MainWindow::on_groupFlushBtn_clicked()
{
    QJsonObject data;
    data["msgid"] = QUERY_GROUP_MSG;
    data["userName"]=AdminCon::getInstance()->getAdminName();

    QJsonDocument doc(data);
    QByteArray json = doc.toJson();
    QSocket::getInstance().sendData(json);
}


void MainWindow::on_addGroupBtn_clicked()
{
    addGroupDia UI(this);
    UI.setModal(this);
    UI.exec();
}


void MainWindow::on_createGroupBtn_clicked()
{
    createGroupDia UI(this);
    UI.setModal(this);
    UI.exec();
}



void MainWindow::on_departFlushBtn_clicked()
{
    QJsonObject data;
    data["msgid"] = QUERY_DEPART_USERS_MSG;
    data["userName"]=AdminCon::getInstance()->getAdminName();

    QJsonDocument doc(data);
    QByteArray json = doc.toJson();
    QSocket::getInstance().sendData(json);
}


void MainWindow::on_groupHistoryBtn_clicked()
{
    QJsonObject data;
    data["msgid"] = GROUP_CHAT_MSG;
    data["recvName"] = AdminCon::getInstance()->getAdminName();
    data["sendName"] = ui->friendNameLabel->text();

    QJsonDocument doc(data);
    QByteArray json = doc.toJson();
    QSocket::getInstance().sendData(json);
}

