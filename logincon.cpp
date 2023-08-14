#include "logincon.h"

#include "qsocket.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

LoginCon::LoginCon(QObject *parent)
    : QObject{parent}
{
    if(!m_model)
    {
        m_model=LoginModel::getInstance();
    }

    connect(m_model,&LoginModel::loginDataUpdated,this,&LoginCon::updateLoginData);
    connect(m_model,&LoginModel::registerDataUpdated,this,&LoginCon::updateRegisterData);
    connect(m_model,&LoginModel::retrieveDataUpdated,this,&LoginCon::updateRegisterData);
}

LoginCon &LoginCon::getInstance()
{
    static LoginCon instance;
    return instance;
}

void LoginCon::updateLoginData(const QByteArray &data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();
    if(0==obj["errcode"].toInt())
    {
        QString ip = obj["ip"].toString();
        QString temp = obj["port"].toString();
        uint16_t port = temp.toULongLong();
        QSocket::getInstance().connectToServer(ip,port);
        emit loginSeccess();
    }
    else
    {
        QString msg=obj["errmsg"].toString();
        emit loginFailed(msg);
    }
}

void LoginCon::updateRegisterData(const QByteArray &data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();
    if(0==obj["errcode"].toInt())
    {
        QString msg=obj["errmsg"].toString();
        emit registerSuccess(msg);
    }
    else
    {
        QString msg=obj["errmsg"].toString();
        emit registerFailed(msg);
    }
}

void LoginCon::updateRetrieveData(const QByteArray &data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();
    if(0==obj["errcode"].toInt())
    {
        QString msg=obj["errmsg"].toString();
        emit retrieveSuccess(msg);
    }
    else
    {
        QString msg=obj["errmsg"].toString();
        emit retrieveFailed(msg);
    }
}
