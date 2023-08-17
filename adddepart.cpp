#include "adddepart.h"
#include "ui_adddepart.h"

#include "admincon.h"
#include "public.h"
#include "qsocket.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>
#include <QDebug>

AddDepart::AddDepart(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddDepart)
{
    ui->setupUi(this);

     this->setWindowFlags(Qt::SplashScreen|Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
}

AddDepart::~AddDepart()
{
    delete ui;
}

void AddDepart::mousePressEvent(QMouseEvent *event)
{
    isPressedWidget = true; // 当前鼠标按下的即是QWidget而非界面上布局的其它控件
    last = event->globalPos();
}

void AddDepart::mouseMoveEvent(QMouseEvent *event)
{
    if (isPressedWidget)
    {
        int dx = event->globalX() - last.x();
        int dy = event->globalY() - last.y();
        last = event->globalPos();
        move(x()+dx, y()+dy);
    }
}

void AddDepart::mouseReleaseEvent(QMouseEvent *event)
{
    int dx = event->globalX() - last.x();
    int dy = event->globalY() - last.y();
    move(x()+dx, y()+dy);
    isPressedWidget = false; // 鼠标松开时，置为false
}

void AddDepart::on_addDepartBtn_clicked()
{
    QJsonObject data;
    data["msgid"] = ADD_DEPART_MSG;
    data["userName"] = AdminCon::getInstance()->getAdminName();
    data["departName"] = ui->addDepartName->text();
    data["userRole"] = "employee";

    QJsonDocument doc(data);
    QByteArray json = doc.toJson();
    QSocket::getInstance().sendData(json);

    qDebug()<<json;

    this->close();
}


void AddDepart::on_toolButton_clicked()
{
    this->close();
}

