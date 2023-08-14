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
};

struct groupMsgInfo
{
    QString groupName;
    QList<QPair<QString,QString>> groupMsg;
};

class GroupModel : public QObject
{
    Q_OBJECT
public:
    explicit GroupModel(QObject *parent = nullptr);

    void recvNewMsg(QByteArray data);
    void recvNewGroups(QByteArray data);

    QList<groupInfo> getGroups() const;
    void setGroups(const QList<groupInfo> &newGroups);

    QList<groupMsgInfo> getGroupMsgs() const;
    void setGroupMsgs(const QList<groupMsgInfo> &newGroupMsgs);

    groupInfo getCurrentGroup() const;
    void setCurrentGroup(const groupInfo &newCurrentGroup);

    void addMsg(const QString& name,const QString&msg);

signals:
    void newMsg(QString groupName);
    void newGroups();
    void showNewUser(QList<QString> list);
private:
    QList<groupInfo> groups;
    QList<groupMsgInfo> groupMsgs;
    groupInfo currentGroup;
};

#endif // GROUPMODEL_H
