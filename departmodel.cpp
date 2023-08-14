#include "departmodel.h"

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


void DepartModel::recvNewDepartMsg(QByteArray data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();
    //qDebug()<<"----";
    if(obj["message"].toString()!="")
    {
        //qDebug()<<"----";
        departMsgInfo temp;
        temp.sendName = obj["sendName"].toString();
        temp.msg = obj["message"].toString();
        departMsgs.push_back(temp);
        setCurrentMsg(temp);
        emit newDepartMsg();
    }
//    else if(obj["chatmsgs"].isArray())
//    {
//        QJsonArray chatmsgArray = obj.value("chatmsgs").toArray();
//        for(const QJsonValue& val:chatmsgArray)
//        {
//            MsgInfo temp;
//            temp.setMsgInfoVal(val["sendName"].toString(),AdminCon::getInstance()->getAdminName(),val["message"].toString());
//            msgInfoList.push_back(temp);
//            emit newDepartMsg();
//        }
//    }
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

