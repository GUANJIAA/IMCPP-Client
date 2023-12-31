#include "addfrienddia.h"
#include "ui_addfrienddia.h"

#include "admincon.h"
#include "public.h"
#include "qsocket.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>


addFriendDia::addFriendDia(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addFriendDia)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::SplashScreen|Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
}

addFriendDia::~addFriendDia()
{
    delete ui;
}

void addFriendDia::mousePressEvent(QMouseEvent *event)
{
    isPressedWidget = true; // 当前鼠标按下的即是QWidget而非界面上布局的其它控件
    last = event->globalPos();
}

void addFriendDia::mouseMoveEvent(QMouseEvent *event)
{
    if (isPressedWidget)
    {
        int dx = event->globalX() - last.x();
        int dy = event->globalY() - last.y();
        last = event->globalPos();
        move(x()+dx, y()+dy);
    }
}

void addFriendDia::mouseReleaseEvent(QMouseEvent *event)
{
    int dx = event->globalX() - last.x();
    int dy = event->globalY() - last.y();
    move(x()+dx, y()+dy);
    isPressedWidget = false; // 鼠标松开时，置为false
}

void addFriendDia::on_toolButton_2_clicked()
{
    this->close();
}


void addFriendDia::on_pushButton_clicked()
{
    QJsonObject data;
    data["msgid"] = ADD_FRIEND_MSG;
    data["userName"] = AdminCon::getInstance()->getAdminName();
    data["peerName"] = ui->addFirendNameLineEdit->text();

    QJsonDocument doc(data);
    QByteArray json = doc.toJson();
    QSocket::getInstance().sendData(json);

    this->close();
}

