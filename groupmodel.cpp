#include "groupmodel.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

GroupModel::GroupModel(QObject *parent)
    : QObject{parent}
{

}

void GroupModel::recvNewMsg(QByteArray data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();
    if(obj["message"].toString()!="")
    {
        bool temp=false;
        for(auto &val:groupMsgs)
        {
            if(val.groupName==obj["groupName"].toString())
            {
                temp=true;
                val.groupMsg.push_back({obj["sendName"].toString(),obj["message"].toString()});
            }
        }
        if(!temp)
        {
            groupMsgInfo msginfo;
            msginfo.groupName=obj["groupName"].toString();
            msginfo.groupMsg.push_back({obj["sendName"].toString(),obj["message"].toString()});
            groupMsgs.push_back(msginfo);
        }
        emit newMsg(obj["groupName"].toString());
    }
//    else if(obj["chatmsgs"].isArray())
//    {
//        QJsonArray chatmsgArray = obj.value("chatmsgs").toArray();
//        for(const QJsonValue& val:chatmsgArray)
//        {
//            MsgInfo temp;
//            temp.setMsgInfoVal(val["sendName"].toString(),AdminCon::getInstance()->getAdminName(),val["message"].toString());
//            msgInfoList.push_back(temp);
//        }
//    }
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
            temp.groupDesc = val["grouDesc"].toString();
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

QList<groupInfo> GroupModel::getGroups() const
{
    return groups;
}

void GroupModel::setGroups(const QList<groupInfo> &newGroups)
{
    groups = newGroups;
}

QList<groupMsgInfo> GroupModel::getGroupMsgs() const
{
    return groupMsgs;
}

void GroupModel::setGroupMsgs(const QList<groupMsgInfo> &newGroupMsgs)
{
    groupMsgs = newGroupMsgs;
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
    for(auto &group:groupMsgs)
    {
        if(group.groupName==currentGroup.groupName)
        {
            group.groupMsg.push_back({name,msg});
        }
    }
}
