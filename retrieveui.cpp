#include "retrieveui.h"
#include "ui_retrieveui.h"

#include "qsocket.h"
#include "logincon.h"
#include "public.h"

#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>

retrieveui::retrieveui(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::retrieveui)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Dialog);

    this->setWindowFlags(Qt::SplashScreen|Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);

    connect(&LoginCon::getInstance(),&LoginCon::retrieveSuccess,this,&retrieveui::onRetrieveSuccess);
    connect(&LoginCon::getInstance(),&LoginCon::retrieveFailed,this,&retrieveui::onRetrieveFailed);

}

retrieveui::~retrieveui()
{
    delete ui;
}

void retrieveui::on_CloseBtn_clicked()
{
    this->close();
}

void retrieveui::mousePressEvent(QMouseEvent *event)
{
    isPressedWidget = true; // 当前鼠标按下的即是QWidget而非界面上布局的其它控件
    last = event->globalPos();
}

void retrieveui::mouseMoveEvent(QMouseEvent *event)
{
    if (isPressedWidget)
    {
        int dx = event->globalX() - last.x();
        int dy = event->globalY() - last.y();
        last = event->globalPos();
        move(x()+dx, y()+dy);
    }
}

void retrieveui::mouseReleaseEvent(QMouseEvent *event)
{
    int dx = event->globalX() - last.x();
    int dy = event->globalY() - last.y();
    move(x()+dx, y()+dy);
    isPressedWidget = false; // 鼠标松开时，置为false
}

void retrieveui::onRetrieveSuccess(QString msg)
{
    QMessageBox::information(this,"Message",msg);
}

void retrieveui::onRetrieveFailed(QString msg)
{
    QMessageBox::information(this,"Message",msg);
}


void retrieveui::on_retrieveBtn_clicked()
{
    QJsonObject data;
    data["msgid"]=RETRIEVE_MSG;
    data["name"] = ui->NameLineEdit->text();
    data["pwd"] = ui->PwdLineEdit->text();
    data["email"]=ui->EmailLineEdit->text();
    data["phone"]=ui->PhoneEdit->text();

    QJsonDocument doc(data);
    QByteArray json = doc.toJson();
    QSocket::getInstance().sendData(json);
}

