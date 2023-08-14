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
}

