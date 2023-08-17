#include "departmodel.h"

#include "admincon.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QDebug>

DepartModel::DepartModel(QObject *parent)
    : QObject{parent}
{

}

void DepartModel::recvNewDepart(QByteArray data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();
    currentDepart.departUsers.clear();
    currentDepart.departName = obj["departName"].toString();
    currentDepart.departDesc = obj["departDesc"].toString();
    QJsonArray groupArray = obj.value("users").toArray();

    for(const QJsonValue& val : groupArray)
    {

        departUserInfo temp;
        temp.userName = val["userName"].toString();
        temp.userEmail = val["userEmail"].toString();
        temp.userPhone = val["userPhone"].toString();
        temp.userRole = val["userRole"].toString();
        currentDepart.departUsers.push_back(temp);
    }

    if(!groupArray.empty())
    {
        emit newDepart();
    }
}

void DepartModel::recvAddDepart(QByteArray data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();

    if(!obj["errcode"].toInt())
    {
        AdminCon::getInstance()->setAdminDepartName(obj["departName"].toString());
        emit addDepart(true);
    }
    else
    {
        emit addDepart(false);
    }
}

void DepartModel::recvQuitDepart(QByteArray data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();

    if(!obj["errcode"].toInt())
    {
        emit quitDepart(true);
    }
    else
    {
        emit quitDepart(false);
    }
}

void DepartModel::recvNewDepartMsg(QByteArray data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();
    if(obj["message"].toString()!="")
    {
        departMsgInfo temp;
        temp.sendName = obj["sendName"].toString();
        temp.msg = obj["message"].toString();
        departMsgs.push_back(temp);
        setCurrentMsg(temp);
        emit newDepartMsg();
    }
    else if(obj["chatmsgs"].isArray())
    {
        departMsgs.clear();
        QJsonArray chatmsgArray = obj.value("chatmsgs").toArray();
        for(const QJsonValue& val:chatmsgArray)
        {
            departMsgInfo temp;
            temp.msgid=val["msgId"].toInt();
            temp.sendName = val["sendName"].toString();
            temp.msg = val["message"].toString();
            departMsgs.push_back(temp);
            setCurrentMsg(temp);
        }
        std::sort(departMsgs.begin(),departMsgs.end());
        if(!chatmsgArray.empty())
        {
            emit recvHistoryMsg();
        }
    }

}

departInfo DepartModel::getCurrentDepart() const
{
    return currentDepart;
}

void DepartModel::setCurrentDepart(const departInfo &newCurrentDepart)
{
    currentDepart = newCurrentDepart;
}

QList<departMsgInfo> DepartModel::getDepartMsgs() const
{
    return departMsgs;
}

void DepartModel::setDepartMsgs(const QList<departMsgInfo> &newDepartMsgs)
{
    departMsgs = newDepartMsgs;
}

departMsgInfo DepartModel::getCurrentMsg() const
{
    return currentMsg;
}

void DepartModel::setCurrentMsg(const departMsgInfo &newCurrentMsg)
{
    currentMsg = newCurrentMsg;
}

void DepartModel::addDepartMsg(QString sendName,QString msg)
{
    departMsgInfo temp;
    temp.sendName=sendName;
    temp.msg=msg;
    departMsgs.push_back(temp);
}

void DepartModel::clear()
{
    currentMsg.sendName="";
    currentMsg.msg="";
    currentDepart.departName="";
    currentDepart.departDesc="";
    currentDepart.departUsers.clear();
    departMsgs.clear();
}

