#include "admininfo.h"
#include "ui_admininfo.h"

#include "admincon.h"
#include "public.h"
#include "qsocket.h"

#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>

adminInfo::adminInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::adminInfo)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::SplashScreen|Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);

    connect(AdminCon::getInstance(),&AdminCon::reviseAdmin,this,&adminInfo::onCommitResult);
}

adminInfo::~adminInfo()
{
    delete ui;
}

void adminInfo::on_toolButton_2_clicked()
{
    this->close();
}

void adminInfo::mousePressEvent(QMouseEvent *event)
{
    isPressedWidget = true; // 当前鼠标按下的即是QWidget而非界面上布局的其它控件
    last = event->globalPos();
}

void adminInfo::mouseMoveEvent(QMouseEvent *event)
{
    if (isPressedWidget)
    {
        int dx = event->globalX() - last.x();
        int dy = event->globalY() - last.y();
        last = event->globalPos();
        move(x()+dx, y()+dy);
    }
}

void adminInfo::mouseReleaseEvent(QMouseEvent *event)
{
    int dx = event->globalX() - last.x();
    int dy = event->globalY() - last.y();
    move(x()+dx, y()+dy);
    isPressedWidget = false; // 鼠标松开时，置为false
}

void adminInfo::on_commitBtn_clicked()
{
    QJsonObject data;
    data["msgid"]=UPDATE_ADMININFO;
    data["userName"] = AdminCon::getInstance()->getAdminName();
    data["userPwd"] = ui->pwdLineEdit->text();
    data["userEmail"] = ui->emailLineEdit->text();
    data["userPhone"] = ui->phoneLineEdit->text();
    data["userDesc"] = ui->descLineEdit->text();
    //qDebug()<<AdminCon::getInstance()->getAdminDepartName();
    data["userDepartName"] = AdminCon::getInstance()->getAdminDepartName();

    QJsonDocument doc(data);
    QByteArray json = doc.toJson();
    QSocket::getInstance().sendData(json);
}

void adminInfo::onCommitResult(QString msg)
{
    QMessageBox::information(this,"Message",msg);
}

