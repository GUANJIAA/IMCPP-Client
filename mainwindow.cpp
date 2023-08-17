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
#include "historychatmsgdia.h"
#include "adddepart.h"
#include "departinfo.h"
#include "groupinfodia.h"

#include <QDebug>
#include <unistd.h>
#include <QTimer>
#include <QThread>
#include <QPushButton>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>
#include <QMessageBox>

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

    connect(&FriendCon::getInstance()->_friendmodel,&FriendModel::recvAddFriend,this,&MainWindow::onRecvAddFriend);
    connect(&FriendCon::getInstance()->_friendmodel,&FriendModel::recvDeleteFriend,this,&MainWindow::onRecvDelFriend);

    connect(&GroupCon::getInstance()->_groupmodel,&GroupModel::recvCreateGroup,this,&MainWindow::onRecvCreateGroup);
    connect(&GroupCon::getInstance()->_groupmodel,&GroupModel::recvAddGroup,this,&MainWindow::onRecvAddGroup);
    connect(&GroupCon::getInstance()->_groupmodel,&GroupModel::recvQuitGroup,this,&MainWindow::onRecvQUitGroup);

    connect(&DepartCon::getInstance()->_departmodel,&DepartModel::createDepart,this,&MainWindow::onRecvCreateDepart);
    connect(&DepartCon::getInstance()->_departmodel,&DepartModel::delDepart,this,&MainWindow::onRecvDelDepart);
    connect(&DepartCon::getInstance()->_departmodel,&DepartModel::addDepart,this,&MainWindow::onRecvAddDepart);
    connect(&DepartCon::getInstance()->_departmodel,&DepartModel::quitDepart,this,&MainWindow::onRecvQuitDepart);
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

void MainWindow::onRecvAddFriend(bool result)
{
    if(result)
    {
        QJsonObject data;
        data["msgid"] = QUERY_FRIEND_MSG;
        data["userName"] = AdminCon::getInstance()->getAdminName();

        QJsonDocument doc(data);
        QByteArray json = doc.toJson();
        QSocket::getInstance().sendData(json);
    }
    else
    {
        QMessageBox::information(this,"message","add friend faild");
    }
}

void MainWindow::onRecvDelFriend(bool result)
{
    if(result)
    {
        ui->friendTextEdit->clear();
        ui->friendTextBro->clear();
        ui->friendNameLabel->clear();

        QJsonObject data;
        data["msgid"] = QUERY_FRIEND_MSG;
        data["userName"] = AdminCon::getInstance()->getAdminName();

        QJsonDocument doc(data);
        QByteArray json = doc.toJson();
        QSocket::getInstance().sendData(json);
    }
    else
    {
        QMessageBox::information(this,"message","del friend faild");
    }
}

void MainWindow::onRecvCreateGroup(bool result,QString groupName)
{
    if(result)
    {
        {
            QJsonObject data;
            data["msgid"] = ADD_GROUP_MSG;
            data["groupName"]=groupName;
            data["userName"]=AdminCon::getInstance()->getAdminName();
            data["userRole"] = "creator";

            QJsonDocument doc(data);
            QByteArray json = doc.toJson();
            QSocket::getInstance().sendData(json);
        }

        QJsonObject data;
        data["msgid"] = QUERY_GROUP_MSG;
        data["userName"]=AdminCon::getInstance()->getAdminName();

        QJsonDocument doc(data);
        QByteArray json = doc.toJson();
        QSocket::getInstance().sendData(json);
    }
    else
    {
        QMessageBox::information(this,"message","create depart faild");
    }
}

void MainWindow::onRecvAddGroup(bool result)
{
    if(result)
    {
        QJsonObject data;
        data["msgid"] = QUERY_GROUP_MSG;
        data["userName"]=AdminCon::getInstance()->getAdminName();

        QJsonDocument doc(data);
        QByteArray json = doc.toJson();
        QSocket::getInstance().sendData(json);
    }
    else
    {
        QMessageBox::information(this,"message","create depart faild");
    }
}

void MainWindow::onRecvQUitGroup(bool result)
{
    if(result)
    {
        ui->groupChatTextBrow->clear();
        ui->groupDescTextBrow->clear();
        ui->groupNameLabel->clear();
        ui->groupUsersListWidget->clear();

        QJsonObject data;
        data["msgid"] = QUERY_GROUP_MSG;
        data["userName"]=AdminCon::getInstance()->getAdminName();

        QJsonDocument doc(data);
        QByteArray json = doc.toJson();
        QSocket::getInstance().sendData(json);
    }
    else
    {
        QMessageBox::information(this,"message","create depart faild");
    }
}

void MainWindow::onRecvCreateDepart(bool result)
{
    if(result)
    {
        QJsonObject data;
        data["msgid"] = QUERY_DEPART_USERS_MSG;
        data["userName"]=AdminCon::getInstance()->getAdminName();

        QJsonDocument doc(data);
        QByteArray json = doc.toJson();
        QSocket::getInstance().sendData(json);
    }
    else
    {
        QMessageBox::information(this,"message","create depart faild");
    }
}

void MainWindow::onRecvDelDepart(bool result)
{
    if(result)
    {
        ui->departChatTextBrow->clear();
        ui->departNameLabel->clear();
        ui->departUserListWidget->clear();
        ui->departSendTextEdit->clear();

        DepartCon::getInstance()->_departmodel.clear();
    }
    else
    {
        QMessageBox::information(this,"message","del depart faild");
    }
}

void MainWindow::onRecvAddDepart(bool result)
{
    if(result)
    {
        {
            QJsonObject data;
            data["msgid"]=UPDATE_ADMININFO;
            data["userName"] = AdminCon::getInstance()->getAdminName();
            data["userPwd"] = AdminCon::getInstance()->getAdminPwd();
            data["userEmail"] = AdminCon::getInstance()->getAdminEmail();
            data["userPhone"] = AdminCon::getInstance()->getAdminPhone();
            data["userDesc"] = AdminCon::getInstance()->getAdminDesc();
            data["userDepartName"] = AdminCon::getInstance()->getAdminDepartName();

            QJsonDocument doc(data);
            QByteArray json = doc.toJson();
            QSocket::getInstance().sendData(json);
        }

        sleep(1);

        {
            QJsonObject data;
            data["msgid"] = QUERY_DEPART_USERS_MSG;
            data["userName"]=AdminCon::getInstance()->getAdminName();

            QJsonDocument doc(data);
            QByteArray json = doc.toJson();
            QSocket::getInstance().sendData(json);
        }

    }
    else
    {
        QMessageBox::information(this,"message","create depart faild");
    }
}

void MainWindow::onRecvQuitDepart(bool result)
{
    if(result)
    {
        ui->departChatTextBrow->clear();
        ui->departNameLabel->clear();
        ui->departUserListWidget->clear();
        ui->departSendTextEdit->clear();
        ui->departNoticeTextBrow->clear();

        DepartCon::getInstance()->_departmodel.clear();

        QJsonObject data;
        data["msgid"]=UPDATE_ADMININFO;
        data["userName"] = AdminCon::getInstance()->getAdminName();
        data["userPwd"] = AdminCon::getInstance()->getAdminPwd();
        data["userEmail"] = AdminCon::getInstance()->getAdminEmail();
        data["userPhone"] = AdminCon::getInstance()->getAdminPhone();
        data["userDesc"] = AdminCon::getInstance()->getAdminDesc();
        data["userDepartName"] = "TEXT";

        QJsonDocument doc(data);
        QByteArray json = doc.toJson();
        QSocket::getInstance().sendData(json);
    }
    else
    {
        QMessageBox::information(this,"message","quit depart faild");
    }
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
    if(FriendCon::getInstance()->getFriendName()->text()=="")
    {
        return ;
    }

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
    if(FriendCon::getInstance()->getFriendName()->text()=="")
    {
        return ;
    }

    QStringList sendNames;
    QStringList messages;
    std::vector<int> vec;
    for(auto &val:FriendCon::getInstance()->_friendmodel.getMsgInfoList())
    {
        if(val.sendName==AdminCon::getInstance()->getAdminName()&&
            val.recvName==FriendCon::getInstance()->currentFriend.userName)
        {
            sendNames.push_back(val.sendName);
            messages.push_back(val.msg);
            vec.push_back(val.msgid);
        }
        else if(val.recvName==AdminCon::getInstance()->getAdminName()&&
                   val.sendName==FriendCon::getInstance()->currentFriend.userName)
        {
            sendNames.push_back(val.sendName);
            messages.push_back(val.msg);
            vec.push_back(val.msgid);
        }
    }
    historyChatMsgDia UI(this);
    UI.setData(vec,sendNames,messages);
    UI.setModal(this);
    UI.exec();
}


void MainWindow::on_groupMenuBtn_clicked()
{
//    QString groupDesc;
//    groupDesc+="群组姓名："+GroupCon::getInstance()->_groupmodel.getCurrentGroup().groupName+"\n";
//    groupDesc+="群组描述："+GroupCon::getInstance()->_groupmodel.getCurrentGroup().groupDesc+"\n";
//    GroupCon::getInstance()->getGroupDescBrow()->setText(groupDesc);

    if(ui->groupNameLabel->text()=="")
    {
        return ;
    }

    GroupInfoDia UI(this);
    UI.setModal(this);
    UI.exec();
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
    if(AdminCon::getInstance()->getAdminDepartName()=="TEXT")
    {
        return ;
    }

    QJsonObject data;
    data["msgid"] = QUERY_DEPART_USERS_MSG;
    data["userName"]=AdminCon::getInstance()->getAdminName();

    QJsonDocument doc(data);
    QByteArray json = doc.toJson();
    QSocket::getInstance().sendData(json);
}


void MainWindow::on_groupHistoryBtn_clicked()
{
    if(ui->groupNameLabel->text()=="")
    {
        return ;
    }

    QStringList sendNames;
    QStringList messages;
    std::vector<int> vec;
    for(auto val:GroupCon::getInstance()->_groupmodel.getCurrentMsg())
    {
        sendNames.push_back(val.sendName);
        messages.push_back(val.message);
        vec.push_back(val.msgId);
    }
    historyChatMsgDia UI(this);
    UI.getDeleteMsgBtn()->setEnabled(false);
    UI.setData(vec,sendNames,messages);
    UI.setChat("Group");
    UI.setModal(this);
    UI.exec();

    //    QJsonObject data;
    //    data["msgid"] = GROUP_CHAT_MSG;
    //    data["recvName"] = AdminCon::getInstance()->getAdminName();
    //    data["sendName"] = ui->friendNameLabel->text();

    //    QJsonDocument doc(data);
    //    QByteArray json = doc.toJson();
    //    QSocket::getInstance().sendData(json);
}


void MainWindow::on_departMenuLabel_clicked()
{
    if(DepartCon::getInstance()->getDepartNameLabel()->text()=="")
    {
        return ;
    }

    DepartInfo UI(this);
    UI.setModal(this);
    UI.exec();
}


void MainWindow::on_addDepartBtn_clicked()
{
    AddDepart UI(this);
    UI.setModal(this);
    UI.exec();
}


void MainWindow::on_historyBtn_clicked()
{
    if(AdminCon::getInstance()->getAdminDepartName()=="TEXT")
    {
        return ;
    }

    QStringList sendNames;
    QStringList messages;
    std::vector<int> vec;
    for(auto val:DepartCon::getInstance()->_departmodel.getDepartMsgs())
    {
        sendNames.push_back(val.sendName);
        messages.push_back(val.msg);
    }
    historyChatMsgDia UI(this);
    UI.getDeleteMsgBtn()->setEnabled(false);
    UI.setData(vec,sendNames,messages);
    UI.setChat("Depart");
    UI.setModal(this);
    UI.exec();
}


