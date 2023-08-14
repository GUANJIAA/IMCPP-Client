#include "friendmodel.h"
#include "admincon.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

FriendModel::FriendModel(QObject *parent)
    : QObject{parent}
{

}

void FriendModel::queryChatMsg(QByteArray data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();
    if(obj["chatmsgs"].isArray())
    {
        QJsonArray chatmsgArray = obj.value("chatmsgs").toArray();
        for(const QJsonValue& val:chatmsgArray)
        {
            MsgInfo temp;
            temp.setMsgInfoVal(val["sendName"].toString(),val["recvName"].toString(),val["message"].toString());
            msgInfoList.push_back(temp);
            emit newMsg();
        }
    }
}

void FriendModel::recvNewMsg(QByteArray data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();
    if(obj["message"].toString()!="")
    {
        MsgInfo temp;
        temp.setMsgInfoVal(obj["sendName"].toString(),AdminCon::getInstance()->getAdminName(),obj["message"].toString());
        msgInfoList.push_back(temp);
        setMsg(temp);
        emit newMsg();
    }
    else if(obj["chatmsgs"].isArray())
    {
        QJsonArray chatmsgArray = obj.value("chatmsgs").toArray();
        for(const QJsonValue& val:chatmsgArray)
        {
            MsgInfo temp;
            temp.setMsgInfoVal(val["sendName"].toString(),AdminCon::getInstance()->getAdminName(),val["message"].toString());
            msgInfoList.push_back(temp);
            emit newMsg();
        }
    }
}

void FriendModel::recvNewFriends(QByteArray data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();
    if(obj["Friend"].isArray())
    {
        friendInfoList.clear();
        QJsonArray FriendArray = obj.value("Friend").toArray();

        for(const QJsonValue& val : FriendArray)
        {
            FriendInfo temp;
            temp.userName=val["friendName"].toString();
            temp.userStatus=val["friendStatus"].toString();
            temp.userEmail=val["friendEmail"].toString();
            temp.userPhone=val["friendPhone"].toString();
            temp.userDesc=val["friendDesc"].toString();
            temp.userDepartName=val["friendDepartName"].toString();
            friendInfoList.push_back(temp);
        }

        if(!FriendArray.empty())
        {
            emit newFriends();
        }
    }
}

QList<FriendInfo> FriendModel::getFriendInfoList() const
{
    return friendInfoList;
}

QList<MsgInfo> FriendModel::getMsgInfoList() const
{
    return msgInfoList;
}

MsgInfo FriendModel::getMsg() const
{
    return msg;
}

void FriendModel::setMsg(const MsgInfo &newMsg)
{
    msg = newMsg;
}

void FriendModel::addMsgInfo(const MsgInfo &msg)
{
    msgInfoList.push_back(msg);
}

