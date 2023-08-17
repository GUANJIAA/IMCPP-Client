#ifndef GROUPMODEL_H
#define GROUPMODEL_H

#include <QObject>
#include <QList>
#include <QString>

struct groupUserInfo
{
    QString userName;
    QString userEmail;
    QString userPhone;
    QString userRole;
};

struct groupInfo
{
    QString groupName;
    QString groupDesc;
    QList<groupUserInfo> groupUsers;

    void clear(){
        groupName=="";
        groupDesc=="";
        groupUsers.clear();
    }

    bool empty(){
        return groupName==""&&groupDesc=="";
    }
};

struct groupMsgInfo
{
    int msgId=-1;
    QString groupName;
    QString sendName;
    QString message;

    bool operator <(const groupMsgInfo& other)const
    {
        return msgId<other.msgId;
    }
};

class GroupModel : public QObject
{
    Q_OBJECT
public:
    explicit GroupModel(QObject *parent = nullptr);

    void recvNewMsg(QByteArray data);
    void recvNewGroups(QByteArray data);

    void recvCreateGroupResult(QByteArray data);
    void recvAddGroupResult(QByteArray data);
    void recvQuitGroupResult(QByteArray data);

    QList<groupInfo> getGroups() const;
    void setGroups(const QList<groupInfo> &newGroups);

    groupInfo getCurrentGroup() const;
    void setCurrentGroup(const groupInfo &newCurrentGroup);

    void addMsg(const QString& name,const QString&msg);

    QList<groupMsgInfo> getCurrentMsg() const;
    void setCurrentMsg(const QList<groupMsgInfo> &newCurrentMsg);

signals:
    void newMsg(QString groupName);
    void newGroups();
    void showNewUser(QList<QString> list);

    // void recvHistroyGroupMsg(bool result);
    void recvCreateGroup(bool result,QString groupName);
    void recvAddGroup(bool result);
    void recvQuitGroup(bool result);

public:
    std::unordered_map<QString,QList<groupMsgInfo>> groupMsgs;
private:
    QList<groupInfo> groups;
    QList<groupMsgInfo> currentMsg;
    groupInfo currentGroup;
};

#endif // GROUPMODEL_H
