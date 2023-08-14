#include "admincon.h"

#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>

AdminCon::AdminCon(QObject *parent)
    : QObject{parent}{}

AdminCon *AdminCon::getInstance()
{
    static AdminCon* instance;
    if(!instance)
    {
        instance=new AdminCon();
    }
    return instance;
}

QString AdminCon::getAdminName() const
{
    return adminName;
}

void AdminCon::setAdminName(const QString &newAdminName)
{
    adminName = newAdminName;
}

QString AdminCon::getAdminPwd() const
{
    return adminPwd;
}

void AdminCon::setAdminPwd(const QString &newAdminPwd)
{
    adminPwd = newAdminPwd;
}

QString AdminCon::getAdminEmail() const
{
    return adminEmail;
}

void AdminCon::setAdminEmail(const QString &newAdminEmail)
{
    adminEmail = newAdminEmail;
}

QString AdminCon::getAdminPhone() const
{
    return adminPhone;
}

void AdminCon::setAdminPhone(const QString &newAdminPhone)
{
    adminPhone = newAdminPhone;
}

QString AdminCon::getAdminDesc() const
{
    return adminDesc;
}

void AdminCon::setAdminDesc(const QString &newAdminDesc)
{
    adminDesc = newAdminDesc;
}

QByteArray AdminCon::getData() const
{
    return data;
}

void AdminCon::setData(const QByteArray &newData)
{
    data = newData;
}

void AdminCon::updateData(QByteArray data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();
    if(0 == obj["errcode"].toInt())
    {
        if(obj["userName"].toString()!="")
        {
            adminName=obj["userName"].toString();
            adminPwd=obj["userPwd"].toString();
            adminEmail=obj["userEmail"].toString();
            adminPhone=obj["userPhone"].toString();
            adminDesc=obj["userDesc"].toString();
            adminDepartName=obj["userDepartName"].toString();
            emit updateAdmin();
        }
    }
}

void AdminCon::reviseAdminResult(QByteArray data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();
    if(0 == obj["errcode"].toInt())
    {
        reviseAdmin(obj["errmsg"].toString());
        adminName=obj["userName"].toString();
        adminPwd=obj["userPwd"].toString();
        adminEmail=obj["userEmail"].toString();
        adminPhone=obj["userPhone"].toString();
        adminDesc=obj["userDesc"].toString();
        adminDepartName=obj["userDepartName"].toString();
        emit updateAdmin();
    }
    else
    {
        reviseAdmin(obj["errmsg"].toString());
    }
}

QString AdminCon::getAdminDepartName() const
{
    return adminDepartName;
}

void AdminCon::setAdminDepartName(const QString &newAdminDepartName)
{
    adminDepartName = newAdminDepartName;
}
