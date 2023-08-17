#include "friendcon.h"

#include "admincon.h"
#include "public.h"

#include <QDebug>
#include <QFont>
#include <QString>
#include <QScrollBar>
#include <qsocket.h>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>

FriendCon::FriendCon(QObject *parent)
    : QObject{parent}{
    connect(&_friendmodel,&FriendModel::newMsg,this,&FriendCon::onNewMessageReceived);
    connect(&_friendmodel,&FriendModel::newFriends,this,&FriendCon::onNewFriendsReceived);
    //connect(&_friendmodel,&FriendModel::recvDeleteFriend,this,&FriendCon::)
    connect(&_friendmodel,&FriendModel::recvHistoryMsg,this,&FriendCon::onHistoryMsg);
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

QString generateChatMessageHTML(const QString& text, bool isSentByMe) {
    QString alignment = isSentByMe ? "right" : "left";
    return "<div style=\"text-align: " + alignment + "; width: 100%;\">" + text + "</div>";
}

void FriendCon::onFriendSelected(QListWidgetItem *friendName)
{
    _friendmodel.setMsgInfoList(_friendmodel.getMsgInfoLists()[friendName->text()]);

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
                   msg+=generateChatMessageHTML(friendInfo.userName+"<br>"+val.msg,false);
                }
                else if(val.recvName==friendName->text()&&val.sendName==AdminCon::getInstance()->getAdminName())
                {
                   msg+=generateChatMessageHTML(AdminCon::getInstance()->getAdminName()+"<br>"+val.msg,true);
                }
            }
            messageBrowser->setHtml(msg);

            QScrollBar*vScrollBar=messageBrowser->verticalScrollBar();
            vScrollBar->setValue(vScrollBar->maximum());
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

    QScrollBar*vScrollBar=messageBrowser->verticalScrollBar();
    vScrollBar->setValue(vScrollBar->maximum());
}

void FriendCon::onFriendSendBtn(bool cliecked)
{
    Q_UNUSED(cliecked)

    if(friendName->text()=="")
    {
        return ;
    }

    MsgInfo temp;
    temp.setMsgInfoVal(-1,AdminCon::getInstance()->getAdminName(),getCurrentFriend().userName,getSendTextEdit()->toPlainText());
    _friendmodel.addMsgInfo(temp);


    QString name=generateChatMessageHTML(temp.sendName,true);
    QString msg=generateChatMessageHTML(temp.msg,true);
    messageBrowser->setHtml(messageBrowser->toHtml()+name+msg);

    QScrollBar*vScrollBar=messageBrowser->verticalScrollBar();
    vScrollBar->setValue(vScrollBar->maximum());

    QJsonObject data;
    data["msgid"]=ONE_CHAT_MSG;
    data["sendName"] = AdminCon::getInstance()->getAdminName();
    data["recvName"] = getCurrentFriend().userName;
    data["message"] = getSendTextEdit()->toPlainText();
    data["isRead"] = false;

    QJsonDocument doc(data);
    QByteArray json = doc.toJson();
    QSocket::getInstance().sendData(json);

    getSendTextEdit()->clear();
}

FriendInfo FriendCon::getCurrentFriend() const
{
    return currentFriend;
}

void FriendCon::setCurrentFriend(const FriendInfo &newCurrentFriend)
{
    currentFriend = newCurrentFriend;
}

void FriendCon::onHistoryMsg(bool result)
{
    if(result)
    {
        QString msg;
        for(auto &val:_friendmodel.getMsgInfoList())
        {
            if(val.sendName==currentFriend.userName&&val.recvName==AdminCon::getInstance()->getAdminName())
            {
                msg+=generateChatMessageHTML(val.sendName+":"+"<br>"+val.msg,false);
            }
            else if(val.recvName==currentFriend.userName&&val.sendName==AdminCon::getInstance()->getAdminName())
            {
                msg+=generateChatMessageHTML(AdminCon::getInstance()->getAdminName()+":"+"<br>"+val.msg,true);
            }
        }
        messageBrowser->setHtml(msg);

        QScrollBar*vScrollBar=messageBrowser->verticalScrollBar();
        vScrollBar->setValue(vScrollBar->maximum());
    }
}

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

