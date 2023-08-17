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

void addGroupDia::mousePressEvent(QMouseEvent *event)
{
    isPressedWidget = true; // 当前鼠标按下的即是QWidget而非界面上布局的其它控件
    last = event->globalPos();
}

void addGroupDia::mouseMoveEvent(QMouseEvent *event)
{
    if (isPressedWidget)
    {
        int dx = event->globalX() - last.x();
        int dy = event->globalY() - last.y();
        last = event->globalPos();
        move(x()+dx, y()+dy);
    }
}

void addGroupDia::mouseReleaseEvent(QMouseEvent *event)
{
    int dx = event->globalX() - last.x();
    int dy = event->globalY() - last.y();
    move(x()+dx, y()+dy);
    isPressedWidget = false; // 鼠标松开时，置为false
}

void addGroupDia::on_pushButton_clicked()
{
    QJsonObject data;
    data["msgid"] = ADD_GROUP_MSG;
    data["userName"] = AdminCon::getInstance()->getAdminName();
    data["groupName"] = ui->groupNameLineEdit->text();
    data["userRole"] = "normal";

    QJsonDocument doc(data);
    QByteArray json = doc.toJson();
    QSocket::getInstance().sendData(json);

    this->close();
}


void addGroupDia::on_toolButton_clicked()
{
    this->close();
}

