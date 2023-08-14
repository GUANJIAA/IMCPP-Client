#include "creategroupdia.h"
#include "ui_creategroupdia.h"

#include "admincon.h"
#include "public.h"
#include "qsocket.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

createGroupDia::createGroupDia(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::createGroupDia)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::SplashScreen|Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
}

createGroupDia::~createGroupDia()
{
    delete ui;
}

void createGroupDia::on_toolButton_2_clicked()
{
    this->close();
}


void createGroupDia::on_pushButton_clicked()
{
    QJsonObject data;
    data["msgid"] = CREATE_GROUP_MSG;
    data["groupName"] = ui->createGroupLineEdit->text();
    data["userName"] = AdminCon::getInstance()->getAdminName();
    data["userRole"] = "creator";

    QJsonDocument doc(data);
    QByteArray json = doc.toJson();
    QSocket::getInstance().sendData(json);
}

