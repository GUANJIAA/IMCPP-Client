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

void createGroupDia::mousePressEvent(QMouseEvent *event)
{
    isPressedWidget = true; // 当前鼠标按下的即是QWidget而非界面上布局的其它控件
    last = event->globalPos();
}

void createGroupDia::mouseMoveEvent(QMouseEvent *event)
{
    if (isPressedWidget)
    {
        int dx = event->globalX() - last.x();
        int dy = event->globalY() - last.y();
        last = event->globalPos();
        move(x()+dx, y()+dy);
    }
}

void createGroupDia::mouseReleaseEvent(QMouseEvent *event)
{
    int dx = event->globalX() - last.x();
    int dy = event->globalY() - last.y();
    move(x()+dx, y()+dy);
    isPressedWidget = false; // 鼠标松开时，置为false
}

void createGroupDia::on_toolButton_2_clicked()
{
    this->close();
}



void createGroupDia::on_createGroupBtn_clicked()
{
    QJsonObject data;
    data["msgid"] = CREATE_GROUP_MSG;
    data["groupName"] = ui->GroupNameLineEdit->text();
    data["groupDesc"] = ui->GroupDescLineEdit->text();

    QJsonDocument doc(data);
    QByteArray json = doc.toJson();
    QSocket::getInstance().sendData(json);

    this->close();
}

