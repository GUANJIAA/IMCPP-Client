#ifndef FRIENDCON_H
#define FRIENDCON_H

#include "friendmodel.h"

#include <QDebug>
#include <QList>
#include <QLabel>
#include <QObject>
#include <QWidget>
#include <QString>
#include <QPushButton>
#include <QListWidget>
#include <QTextBrowser>

class FriendCon : public QObject
{
    Q_OBJECT
public:
    explicit FriendCon(QObject *parent = nullptr);

    static FriendCon* getInstance();

    QListWidget *getFriendsList() const;
    void setFriendsList(QListWidget *newFriendsList);

    QTextBrowser *getMessageBrowser() const;
    void setMessageBrowser(QTextBrowser *newMessageBrowser);

    QLabel *getFriendName() const;
    void setFriendName(QLabel *newFriendName);

    QTextEdit *getSendTextEdit() const;
    void setSendTextEdit(QTextEdit *newSendTextEdit);

    QPushButton *getSendPushBtn() const;
    void setSendPushBtn(QPushButton *newSendPushBtn);

    FriendInfo getCurrentFriend() const;
    void setCurrentFriend(const FriendInfo &newCurrentFriend);

    FriendModel friendmodel() const;

signals:
    void showNewFriends(const QList<QString>&friends);
public slots:
    void onHistoryMsg(bool result);
    void onNewMessageReceived();
    void onNewFriendsReceived();

    void onFriendSelected(QListWidgetItem *friendName);
    void onFriendSendBtn(bool cliecked);

private:
    QLabel* friendName;
    QListWidget *friendsList;
    QTextBrowser* messageBrowser;
    QTextEdit* sendTextEdit;
    QPushButton* sendPushBtn;

public:
    FriendModel _friendmodel;
    FriendInfo currentFriend;
};

#endif // FRIENDCON_H
