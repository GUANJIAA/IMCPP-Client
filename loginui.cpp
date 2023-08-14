#include "loginui.h"
#include "ui_loginui.h"

#include "qsocket.h"
#include "public.h"
#include "registerui.h"
#include "retrieveui.h"
#include "logincon.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QDebug>
#include <QPushButton>
#include <QMessageBox>

LoginUI::LoginUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginUI)
{
    this->setWindowFlags(Qt::SplashScreen|Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
    ui->setupUi(this);

//    ui->LoginBtn->setStyleSheet("QPushButton {color: #0095FF; }");
//    ui->RegisterBtn->setStyleSheet("QPushButton {color: #0095FF; }");
//    ui->RetrieveBtn->setStyleSheet("QPushButton {color: #0095FF; }");
//    ui->LoginBtn->setStyleSheet("QPushButton { background-color: #F0F0F0;border-radius: 15px; }");

    ui->CloseBtn->setCursor(Qt::PointingHandCursor);
    ui->LoginBtn->setCursor(Qt::PointingHandCursor);

    // 设置按钮透明并改变鼠标手势
    ui->RegisterBtn->setStyleSheet("background-color: transparent;");
    ui->RegisterBtn->setCursor(Qt::PointingHandCursor);
    ui->RetrieveBtn->setStyleSheet("background-color: transparent;");
    ui->RetrieveBtn->setCursor(Qt::PointingHandCursor);

    ui->pwdLineEdit->setEchoMode(QLineEdit::Password);

    connect(&LoginCon::getInstance(),&LoginCon::loginSeccess,this,&LoginUI::onLoginSuccess);
    connect(&LoginCon::getInstance(),&LoginCon::loginFailed,this,&LoginUI::onLoginFailed);
}

LoginUI::~LoginUI()
{
    delete ui;
}

void LoginUI::mousePressEvent(QMouseEvent *event)
{
    isPressedWidget = true; // 当前鼠标按下的即是QWidget而非界面上布局的其它控件
    last = event->globalPos();
}

void LoginUI::mouseMoveEvent(QMouseEvent *event)
{
    if (isPressedWidget)
    {
        int dx = event->globalX() - last.x();
        int dy = event->globalY() - last.y();
        last = event->globalPos();
        move(x()+dx, y()+dy);
    }
}

void LoginUI::mouseReleaseEvent(QMouseEvent *event)
{
    int dx = event->globalX() - last.x();
    int dy = event->globalY() - last.y();
    move(x()+dx, y()+dy);
    isPressedWidget = false; // 鼠标松开时，置为false
}

void LoginUI::showwidget()
{
    this->show();
}

void LoginUI::closewidget()
{
    this->close();
}

void LoginUI::on_CloseBtn_clicked()
{
    QCoreApplication::quit();
}


void LoginUI::on_LoginBtn_clicked()
{
    QJsonObject data;
    data["msgid"]=LOGIN_MSG;
    data["name"] = ui->nameLineEdit->text();
    data["pwd"] = ui->pwdLineEdit->text();

    QJsonDocument doc(data);
    QByteArray json = doc.toJson();
    QSocket::getInstance().sendData(json);
}


void LoginUI::on_RegisterBtn_clicked()
{
    registerui UI(this);
    UI.setModal(true);
    UI.exec();
}


void LoginUI::on_RetrieveBtn_clicked()
{
    retrieveui UI(this);
    UI.setModal(true);
    UI.exec();
}

void LoginUI::onLoginSuccess()
{
    QJsonObject data;
    data["msgid"]=NEW_CONNECTION;
    data["userName"] = ui->nameLineEdit->text();
    data["pwd"] = ui->pwdLineEdit->text();

    QJsonDocument doc(data);
    QByteArray json = doc.toJson();
    QSocket::getInstance().sendData(json);

    emit switchToMainWindows();
    this->close();
}

void LoginUI::onLoginFailed(QString msg)
{
    QMessageBox::information(this,"Message",msg);
}

