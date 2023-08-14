#include "addgroupdia.h"
#include "ui_addgroupdia.h"

#include "admincon.h"
#include "public.h"
#include "qsocket.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

addGroupDia::addGroupDia(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addGroupDia)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::SplashScreen|Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
}

addGroupDia::~addGroupDia()
{
    delete ui;
}

void addGroupDia::on_pushButton_clicked()
{
    QJsonObject data;
    data["msgid"] = ADD_GROUP_MSG;
    data["userName"] = AdminCon::getInstance()->getAdminName();
    data["groupDesc"] = "TEXT";
    data["groupName"] = ui->groupNameLineEdit->text();
    data["userRole"] = "normal";

    QJsonDocument doc(data);
    QByteArray json = doc.toJson();
    QSocket::getInstance().sendData(json);
}


void addGroupDia::on_toolButton_clicked()
{
    this->close();
}

