#include "friendcon.h"

#include "admincon.h"
#include "public.h"

#include <QDebug>
#include <QFont>
#include <QString>
#include <qsocket.h>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>

FriendCon::FriendCon(QObject *parent)
    : QObject{parent}{
    connect(&_friendmodel,&FriendModel::newMsg,this,&FriendCon::onNewMessageReceived);
    connect(&_friendmodel,&FriendModel::newFriends,this,&FriendCon::onNewFriendsReceived);
}

FriendCon *FriendCon::getInstance()
{
    static FriendCon*instance;
    if(!instance)
    {
        instance=new FriendCon();
    }
    return instance;
}


void FriendCon::onNewFriendsReceived()
{
    QList<QString> list;
    for(auto temp : _friendmodel.getFriendInfoList())
    {
        list.push_back(temp.userName);
    }
    emit showNewFriends(list);
}

/*
void FriendCon::setFriendInfo(const QList<FriendInfo> &val)
{
    this->friendInfoList=val;
}
*/
QString generateChatMessageHTML(const QString& text, bool isSentByMe) {
    QString alignment = isSentByMe ? "right" : "left";
    return "<div style=\"text-align: " + alignment + "; width: 100%;\">" + text + "</div>";
}

void FriendCon::onFriendSelected(QListWidgetItem *friendName)
{
    for(const auto &friendInfo:_friendmodel.getFriendInfoList())
    {
        if(friendInfo.userName==friendName->text())
        {
            this->friendName->setText(friendName->text());
            QString msg;
            for(auto &val:_friendmodel.getMsgInfoList())
            {
                if(val.sendName==friendName->text()&&val.recvName==AdminCon::getInstance()->getAdminName())
                {
                   msg+=generateChatMessageHTML(friendInfo.userName+":"+"<br>"+val.msg,false);
                }
                else if(val.recvName==friendName->text()&&val.sendName==AdminCon::getInstance()->getAdminName())
                {
                   msg+=generateChatMessageHTML(AdminCon::getInstance()->getAdminName()+":"+"<br>"+val.msg,true);
                }
            }
            messageBrowser->setHtml(msg);
            setCurrentFriend(friendInfo);
            break;
        }
    }
}

void FriendCon::onNewMessageReceived()
{
    if(_friendmodel.getMsg().sendName!=getCurrentFriend().userName)
    {
        return ;
    }
    MsgInfo temp = _friendmodel.getMsg();
    QString name=generateChatMessageHTML(temp.sendName,false);
    QString msg=generateChatMessageHTML(temp.msg,false);
    messageBrowser->setHtml(messageBrowser->toHtml()+name+msg);
}

void FriendCon::onFriendSendBtn(bool cliecked)
{
    Q_UNUSED(cliecked)

    MsgInfo temp;
    temp.setMsgInfoVal(AdminCon::getInstance()->getAdminName(),getCurrentFriend().userName,getSendTextEdit()->toPlainText());
    _friendmodel.addMsgInfo(temp);

    QString name=generateChatMessageHTML(temp.sendName,true);
    QString msg=generateChatMessageHTML(temp.msg,true);
    messageBrowser->setHtml(messageBrowser->toHtml()+name+msg);

    getSendTextEdit()->clear();

    QJsonObject data;
    data["msgid"]=ONE_CHAT_MSG;
    data["sendName"] = temp.sendName;
    data["recvName"] = temp.recvName;
    data["message"] = temp.msg;
    data["isRead"] = false;

    QJsonDocument doc(data);
    QByteArray json = doc.toJson();
    QSocket::getInstance().sendData(json);
}

FriendInfo FriendCon::getCurrentFriend() const
{
    return currentFriend;
}

void FriendCon::setCurrentFriend(const FriendInfo &newCurrentFriend)
{
    currentFriend = newCurrentFriend;
}

/*
void FriendCon::recvFriendsName(QByteArray data)
{
    if()
    setFriendList
}
*/
/*
void FriendCon::onNewMessageReceived(const QString &tempSendName, const QString &tempRecvName, const QString &tempMsg)
{
    MsgInfo temp(tempSendName,tempRecvName,tempMsg);
    msgInfoList.push_back(temp);

    QString name=generateMessageHTML(temp.sendName,false);
    QString msg=generateMessageHTML(temp.msg,false);
    messageBrowser->setHtml(messageBrowser->toHtml()+name+msg);
}
*/
QPushButton *FriendCon::getSendPushBtn() const
{
    return sendPushBtn;
}

void FriendCon::setSendPushBtn(QPushButton *newSendPushBtn)
{
    sendPushBtn = newSendPushBtn;
}

QTextEdit *FriendCon::getSendTextEdit() const
{
    return sendTextEdit;
}

void FriendCon::setSendTextEdit(QTextEdit *newSendTextEdit)
{
    sendTextEdit = newSendTextEdit;
}


QLabel *FriendCon::getFriendName() const
{
    return friendName;
}

void FriendCon::setFriendName(QLabel *newFriendName)
{
    friendName = newFriendName;
}

QTextBrowser *FriendCon::getMessageBrowser() const
{
    return messageBrowser;
}

void FriendCon::setMessageBrowser(QTextBrowser *newMessageBrowser)
{
    messageBrowser = newMessageBrowser;
}

QListWidget *FriendCon::getFriendsList() const
{
    return friendsList;
}

void FriendCon::setFriendsList(QListWidget *newFriendsList)
{
    friendsList = newFriendsList;
}

