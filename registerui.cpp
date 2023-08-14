#include "registerui.h"
#include "ui_registerui.h"

#include "qsocket.h"
#include "logincon.h"
#include "public.h"

#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>


registerui::registerui(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::registerui)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Dialog);

    this->setWindowFlags(Qt::SplashScreen|Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);

    connect(&LoginCon::getInstance(),&LoginCon::registerSuccess,this,&registerui::onRegisterSuccessed);
    connect(&LoginCon::getInstance(),&LoginCon::registerFailed,this,&registerui::onRegisterSuccessed);
}

registerui::~registerui()
{
    delete ui;
}

void registerui::on_CloseBtn_clicked()
{
    this->close();
}

void registerui::mousePressEvent(QMouseEvent *event)
{
    isPressedWidget = true; // 当前鼠标按下的即是QWidget而非界面上布局的其它控件
    last = event->globalPos();
}

void registerui::mouseMoveEvent(QMouseEvent *event)
{
    if (isPressedWidget)
    {
        int dx = event->globalX() - last.x();
        int dy = event->globalY() - last.y();
        last = event->globalPos();
        move(x()+dx, y()+dy);
    }
}

void registerui::mouseReleaseEvent(QMouseEvent *event)
{
    int dx = event->globalX() - last.x();
    int dy = event->globalY() - last.y();
    move(x()+dx, y()+dy);
    isPressedWidget = false; // 鼠标松开时，置为false
}

void registerui::on_registerBtn_clicked()
{
    QJsonObject data;
    data["msgid"]=REGISTER_MSG;
    data["name"] = ui->NameLineEdit->text();
    data["pwd"] = ui->PwdLineEdit->text();
    data["email"]=ui->EmailLineEdit->text();
    data["phone"]=ui->PhoneEdit->text();

    QJsonDocument doc(data);
    QByteArray json = doc.toJson();
    QSocket::getInstance().sendData(json);
}

void registerui::onRegisterSuccessed(QString msg)
{
    QMessageBox::information(this,"Message",msg);
}

void registerui::onRegisterFailed(QString msg)
{
    QMessageBox::information(this,"Message",msg);
}

