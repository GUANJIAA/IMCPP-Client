#include "friendinfo.h"
#include "ui_friendinfo.h"

#include "friendcon.h"
#include "qsocket.h"
#include "admincon.h"
#include "public.h"

#include <QFont>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


friendInfo::friendInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::friendInfo)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::SplashScreen|Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);

    ui->deleteFriendBtn->setStyleSheet("color:red");

    if(FriendCon::getInstance()->getCurrentFriend().userName!="")
    {
        ui->menuName->setText(FriendCon::getInstance()->getCurrentFriend().userName);
        ui->menuEmail->setText(FriendCon::getInstance()->getCurrentFriend().userEmail);
        ui->menuPhone->setText(FriendCon::getInstance()->getCurrentFriend().userPhone);
        ui->menuStatus->setText(FriendCon::getInstance()->getCurrentFriend().userStatus);
        ui->menuDesc->setText(FriendCon::getInstance()->getCurrentFriend().userDesc);
        ui->menuDepartName->setText(FriendCon::getInstance()->getCurrentFriend().userDepartName);
    }
}

friendInfo::~friendInfo()
{

    delete ui;
}

void friendInfo::mousePressEvent(QMouseEvent *event)
{
    isPressedWidget = true; // 当前鼠标按下的即是QWidget而非界面上布局的其它控件
    last = event->globalPos();
}

void friendInfo::mouseMoveEvent(QMouseEvent *event)
{
    if (isPressedWidget)
    {
        int dx = event->globalX() - last.x();
        int dy = event->globalY() - last.y();
        last = event->globalPos();
        move(x()+dx, y()+dy);
    }
}

void friendInfo::mouseReleaseEvent(QMouseEvent *event)
{
    int dx = event->globalX() - last.x();
    int dy = event->globalY() - last.y();
    move(x()+dx, y()+dy);
    isPressedWidget = false; // 鼠标松开时，置为false
}

void friendInfo::on_closeBtn_clicked()
{
    this->close();
}


void friendInfo::on_deleteFriendBtn_clicked()
{
    QJsonObject data;
    data["msgid"] = DEL_FRIEND_MSG;
    data["userName"] = AdminCon::getInstance()->getAdminName();
    data["peerName"] = ui->menuName->text();

    QJsonDocument doc(data);
    QByteArray json = doc.toJson();
    QSocket::getInstance().sendData(json);
}

