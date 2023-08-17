#ifndef DEPARTMODEL_H
#define DEPARTMODEL_H

#include <QObject>
#include <QString>
#include <QList>

struct departUserInfo
{
    QString userName;
    QString userEmail;
    QString userPhone;
    QString userRole;
};

struct departInfo
{
    QString departName;
    QString departDesc;
    QList<departUserInfo> departUsers;
};

struct departMsgInfo
{
    int msgid=-1;
    QString sendName;
    QString msg;

    bool operator<(const departMsgInfo&other)const
    {
        return msgid<other.msgid;
    }
};

class DepartModel : public QObject
{
    Q_OBJECT
public:
    explicit DepartModel(QObject *parent = nullptr);

    void recvNewDepartMsg(QByteArray data);
    void recvNewDepart(QByteArray data);
    void recvAddDepart(QByteArray data);
    void recvQuitDepart(QByteArray data);

    departInfo getCurrentDepart() const;
    void setCurrentDepart(const departInfo &newCurrentDepart);

    QList<departMsgInfo> getDepartMsgs() const;
    void setDepartMsgs(const QList<departMsgInfo> &newDepartMsgs);

    departMsgInfo getCurrentMsg() const;
    void setCurrentMsg(const departMsgInfo &newCurrentMsg);

    void addDepartMsg(QString sendName,QString msg);

    void clear();

signals:
    void newDepart();
    void newDepartMsg();
    void recvHistoryMsg();
    void createDepart(bool result);
    void delDepart(bool result);
    void addDepart(bool result);
    void quitDepart(bool result);
private:
    departInfo currentDepart;
    QList<departMsgInfo> departMsgs;
    departMsgInfo currentMsg;
};

#endif // DEPARTMODEL_H
