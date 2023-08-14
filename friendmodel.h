#ifndef FRIENDMODEL_H
#define FRIENDMODEL_H

#include <QObject>

#include <QString>
#include <QByteArray>

struct FriendInfo{
    QString userName="";
    QString userEmail="xxxx";
    QString userPhone="xxxx";
    QString userStatus="xxxx";
    QString userDesc;
    QString userDepartName;

    bool isEmpty()
    {
        return !(userName!=""&&
                 userEmail!=""&&
                 userPhone!=""&&
                 userStatus!="");
    }
};

struct MsgInfo{
    QString sendName="";
    QString recvName="";
    QString msg="";
    bool isRead=false;

    void setMsgInfoVal(const QString &tempSendName,const QString &tempRecvName,const QString &tempMsg)
    {
        sendName=tempSendName;
        recvName=tempRecvName;
        msg=tempMsg;
    }

    bool isEmpty()
    {
        return sendName==""||
               recvName==""||
               msg=="";
    }
};

class FriendModel : public QObject
{
    Q_OBJECT
public:
    explicit FriendModel(QObject *parent = nullptr);

    void queryChatMsg(QByteArray data);
    void recvNewMsg(QByteArray data);
    void recvNewFriends(QByteArray data);

    QList<FriendInfo> getFriendInfoList() const;

    QList<MsgInfo> getMsgInfoList() const;

    MsgInfo getMsg() const;
    void setMsg(const MsgInfo &newMsg);

    FriendInfo getCurrentFriend() const;
    void setCurrentFriend(const FriendInfo &newCurrentFriend);

    void addMsgInfo(const MsgInfo & msg);

signals:
    void newMsg();
    void newFriends();
private:
    QList<FriendInfo> friendInfoList;
    QList<MsgInfo> msgInfoList;
    MsgInfo msg;
};

#endif // FRIENDMODEL_H
