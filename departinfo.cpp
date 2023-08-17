#include "departinfo.h"
#include "ui_departinfo.h"

#include "public.h"
#include "qsocket.h"
#include "admincon.h"
#include "departcon.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>

DepartInfo::DepartInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DepartInfo)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::SplashScreen|Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);

    if(DepartCon::getInstance()->getDepartNameLabel()->text()!="")
    {
        ui->DepartNameLabel->setText(DepartCon::getInstance()->_departmodel.getCurrentDepart().departName);
        ui->DepartDescLabel->setText(DepartCon::getInstance()->_departmodel.getCurrentDepart().departDesc);
    }
}

DepartInfo::~DepartInfo()
{
    delete ui;
}

void DepartInfo::mousePressEvent(QMouseEvent *event)
{
    isPressedWidget = true; // 当前鼠标按下的即是QWidget而非界面上布局的其它控件
    last = event->globalPos();
}

void DepartInfo::mouseMoveEvent(QMouseEvent *event)
{
    if (isPressedWidget)
    {
        int dx = event->globalX() - last.x();
        int dy = event->globalY() - last.y();
        last = event->globalPos();
        move(x()+dx, y()+dy);
    }
}

void DepartInfo::mouseReleaseEvent(QMouseEvent *event)
{
    int dx = event->globalX() - last.x();
    int dy = event->globalY() - last.y();
    move(x()+dx, y()+dy);
    isPressedWidget = false; // 鼠标松开时，置为false
}

void DepartInfo::on_quitDepartBtn_clicked()
{
    QJsonObject data;
    data["msgid"] = QUIT_DEPART_MSG;
    data["userName"] = AdminCon::getInstance()->getAdminName();
    data["departName"] = ui->DepartNameLabel->text();

    QJsonDocument doc(data);
    QByteArray json = doc.toJson();
    QSocket::getInstance().sendData(json);

    // qDebug()<<json;

    this->close();
}


void DepartInfo::on_toolButton_clicked()
{
    this->close();
}

