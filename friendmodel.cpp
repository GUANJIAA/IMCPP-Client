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
            temp.setMsgInfoVal(val["msgid"].toInt(),val["sendName"].toString(),val["recvName"].toString(),val["message"].toString());
            msgInfoList.push_back(temp);
        }
    }

    if(obj["errcode"].toInt()==0)
    {
        emit recvHistoryMsg(true);
    }
    else
    {
        emit recvHistoryMsg(false);
    }
}

void FriendModel::recvNewMsg(QByteArray data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();
    if(obj["message"].toString()!="")
    {
        MsgInfo temp;
        temp.setMsgInfoVal(-1,obj["sendName"].toString(),AdminCon::getInstance()->getAdminName(),obj["message"].toString());
        msgInfoLists[obj["sendName"].toString()].push_back(temp);
        setMsg(temp);
        emit newMsg();
    }
    else if(obj["chatmsgs"].isArray())
    {
        msgInfoList.clear();
        QJsonArray chatmsgArray = obj.value("chatmsgs").toArray();
        QString friendName;
        for(const QJsonValue& val:chatmsgArray)
        {
            MsgInfo temp;
            temp.setMsgInfoVal(val["msgId"].toInt(),val["sendName"].toString(),val["recvName"].toString(),val["message"].toString());
            if(AdminCon::getInstance()->getAdminName()==val["sendName"].toString())
            {
                friendName=val["recvName"].toString();
            }
            msgInfoList.push_back(temp);
        }
        std::sort(msgInfoList.begin(),msgInfoList.end());
        msgInfoLists[friendName]=msgInfoList;
        if(!chatmsgArray.empty())
        {
            emit recvHistoryMsg(true);
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

void FriendModel::recvAddFriendResult(QByteArray data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();
    if(obj["errcode"]==0)
    {
        emit recvAddFriend(true);
    }
    else
    {
        emit recvAddFriend(false);
    }
}

void FriendModel::recvDeleteFriendResult(QByteArray data)
{
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject obj = doc.object();
        if(obj["errcode"]==0)
        {
            emit recvDeleteFriend(true);
        }
        else
        {
            emit recvDeleteFriend(false);
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
    msgInfoLists[msg.recvName].push_back(msg);
}

void FriendModel::setMsgInfoList(const QList<MsgInfo> &newMsgInfoList)
{
    msgInfoList = newMsgInfoList;
}

std::unordered_map<QString, QList<MsgInfo> > FriendModel::getMsgInfoLists() const
{
    return msgInfoLists;
}

void FriendModel::setMsgInfoLists(const std::unordered_map<QString, QList<MsgInfo> > &newMsgInfoLists)
{
    msgInfoLists = newMsgInfoLists;
}

