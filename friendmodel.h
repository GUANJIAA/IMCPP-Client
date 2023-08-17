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
    int msgid;
    QString sendName="";
    QString recvName="";
    QString msg="";
    bool isRead=false;

    void setMsgInfoVal(int tempMsgid,const QString &tempSendName,const QString &tempRecvName,const QString &tempMsg)
    {
        msgid=tempMsgid;
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

    bool operator<(const MsgInfo&other)const{
        return msgid<other.msgid;
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
    void recvAddFriendResult(QByteArray data);
    void recvDeleteFriendResult(QByteArray data);

    QList<FriendInfo> getFriendInfoList() const;

    QList<MsgInfo> getMsgInfoList() const;

    MsgInfo getMsg() const;
    void setMsg(const MsgInfo &newMsg);

    FriendInfo getCurrentFriend() const;
    void setCurrentFriend(const FriendInfo &newCurrentFriend);

    void addMsgInfo(const MsgInfo & msg);

    void setMsgInfoList(const QList<MsgInfo> &newMsgInfoList);

    std::unordered_map<QString, QList<MsgInfo> > getMsgInfoLists() const;
    void setMsgInfoLists(const std::unordered_map<QString, QList<MsgInfo> > &newMsgInfoLists);

signals:
    void recvHistoryMsg(bool result);
    void recvDeleteMsg(bool result);
    void recvAddFriend(bool result);
    void recvDeleteFriend(bool result);
    void newMsg();
    void newFriends();
private:
    QList<FriendInfo> friendInfoList;
    QList<MsgInfo> msgInfoList;
    std::unordered_map<QString,QList<MsgInfo>> msgInfoLists;
    MsgInfo msg;
};

#endif // FRIENDMODEL_H
