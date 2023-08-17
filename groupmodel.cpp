#include "groupmodel.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

GroupModel::GroupModel(QObject *parent)
    : QObject{parent}{}

void GroupModel::recvNewMsg(QByteArray data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();
    if(obj["message"].toString()!="")
    {
        groupMsgInfo msgInfo;
        msgInfo.msgId=obj["msgId"].toInt();
        msgInfo.groupName=obj["groupName"].toString();
        msgInfo.sendName=obj["sendName"].toString();
        msgInfo.message=obj["message"].toString();
        if(currentGroup.groupName==msgInfo.groupName)
        {
            currentMsg.push_back(msgInfo);
        }
        groupMsgs[msgInfo.groupName]=currentMsg;
        emit newMsg(msgInfo.groupName);
    }
    else if(obj["chatmsgs"].isArray())
    {
        QJsonArray chatmsgArray = obj.value("chatmsgs").toArray();
        groupMsgs.clear();
        currentMsg.clear();
        for(const QJsonValue& val:chatmsgArray)
        {
            groupMsgInfo msgInfo;
            msgInfo.msgId=val["msgId"].toInt();
            msgInfo.groupName=val["groupName"].toString();
            msgInfo.sendName=val["sendName"].toString();
            msgInfo.message=val["message"].toString();
            if(currentGroup.groupName==msgInfo.groupName)
            {
                currentMsg.push_back(msgInfo);
            }
            groupMsgs[msgInfo.groupName].push_back(msgInfo);
        }
        for(auto &val:groupMsgs)
        {
            std::sort(val.second.begin(),val.second.end());
        }
        if(!chatmsgArray.empty())
        {

            emit newMsg(currentGroup.groupName);
        }
    }
}

void GroupModel::recvNewGroups(QByteArray data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();
    if(obj["groups"].isArray())
    {
        groups.clear();
        QJsonArray groupArray = obj.value("groups").toArray();
        for(const QJsonValue& val : groupArray)
        {
            groupInfo temp;
            temp.groupName = val["groupName"].toString();
            temp.groupDesc = val["groupDesc"].toString();
            if(val["users"].isArray())
            {
                QJsonArray usersArray = val["users"].toArray();
                for(const QJsonValue&user:usersArray)
                {
                    groupUserInfo userInfo;
                    userInfo.userName=user["userName"].toString();
                    userInfo.userEmail=user["userEmail"].toString();
                    userInfo.userPhone=user["userPhone"].toString();
                    userInfo.userRole=user["userRole"].toString();
                    temp.groupUsers.push_back(userInfo);
                }
            }
            groups.push_back(temp);
        }

        if(!groupArray.empty())
        {
            emit newGroups();
        }
    }
}

void GroupModel::recvCreateGroupResult(QByteArray data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();
    if(!obj["errcode"].toInt())
    {
        emit recvCreateGroup(true,obj["groupName"].toString());
    }
    else
    {
        emit recvCreateGroup(false,"");
    }
}

void GroupModel::recvAddGroupResult(QByteArray data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();
    if(!obj["errcode"].toInt())
    {
        emit recvAddGroup(true);
    }
    else
    {
        emit recvAddGroup(false);
    }
}

void GroupModel::recvQuitGroupResult(QByteArray data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();
    if(!obj["errcode"].toInt())
    {
        emit recvQuitGroup(true);
    }
    else
    {
        emit recvQuitGroup(false);
    }
}

QList<groupInfo> GroupModel::getGroups() const
{
    return groups;
}

void GroupModel::setGroups(const QList<groupInfo> &newGroups)
{
    groups = newGroups;
}

groupInfo GroupModel::getCurrentGroup() const
{
    return currentGroup;
}

void GroupModel::setCurrentGroup(const groupInfo &newCurrentGroup)
{
    currentGroup = newCurrentGroup;
}

void GroupModel::addMsg(const QString &name, const QString &msg)
{
    groupMsgInfo temp;
    QString groupName=currentGroup.groupName;
    temp.groupName=groupName;
    temp.sendName=name;
    temp.message=msg;
    currentMsg.push_back(temp);
    groupMsgs["groupName"].push_back(temp);
}

QList<groupMsgInfo> GroupModel::getCurrentMsg() const
{
    return currentMsg;
}

void GroupModel::setCurrentMsg(const QList<groupMsgInfo> &newCurrentMsg)
{
    currentMsg = newCurrentMsg;
}
