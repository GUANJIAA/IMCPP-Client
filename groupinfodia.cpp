#include "groupinfodia.h"
#include "ui_groupinfodia.h"

#include "admincon.h"
#include "groupcon.h"
#include "public.h"
#include "qsocket.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>

GroupInfoDia::GroupInfoDia(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GroupInfoDia)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::SplashScreen|Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);

    ui->groupNameLabel->setText(GroupCon::getInstance()->_groupmodel.getCurrentGroup().groupName);
    ui->groupDescLabel->setText(GroupCon::getInstance()->_groupmodel.getCurrentGroup().groupDesc);
}

GroupInfoDia::~GroupInfoDia()
{
    delete ui;
}

void GroupInfoDia::mousePressEvent(QMouseEvent *event)
{
    isPressedWidget = true; // 当前鼠标按下的即是QWidget而非界面上布局的其它控件
    last = event->globalPos();
}

void GroupInfoDia::mouseMoveEvent(QMouseEvent *event)
{
    if (isPressedWidget)
    {
        int dx = event->globalX() - last.x();
        int dy = event->globalY() - last.y();
        last = event->globalPos();
        move(x()+dx, y()+dy);
    }
}

void GroupInfoDia::mouseReleaseEvent(QMouseEvent *event)
{
    int dx = event->globalX() - last.x();
    int dy = event->globalY() - last.y();
    move(x()+dx, y()+dy);
    isPressedWidget = false; // 鼠标松开时，置为false
}

void GroupInfoDia::on_quitGroupBtn_clicked()
{
    QJsonObject data;
    data["msgid"] = QUIT_GROUP_MSG;
    data["userName"] = AdminCon::getInstance()->getAdminName();
    data["groupName"] = ui->groupNameLabel->text();

    QJsonDocument doc(data);
    QByteArray json = doc.toJson();
    QSocket::getInstance().sendData(json);

    qDebug()<<json;

    this->close();
}


void GroupInfoDia::on_toolButton_clicked()
{
    this->close();
}

