#include "groupcon.h"

#include "public.h"
#include "admincon.h"

#include <QDebug>
#include "qsocket.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>
#include <QScrollBar>

QString generateGroupMessageHTML(const QString& text, bool isSentByMe) {
    QString alignment = isSentByMe ? "right" : "left";

    return "<div style=\"text-align: " + alignment + "; width: 100%;\">" + text + "</div>";
}

GroupCon::GroupCon(QObject *parent)
    : QObject{parent}
{
    connect(&_groupmodel,&GroupModel::newGroups,this,&GroupCon::onNewGroupsReceived);
    connect(&_groupmodel,&GroupModel::newMsg,this,&GroupCon::onNewGroupMessageReceived);
}

GroupCon *GroupCon::getInstance()
{
    static GroupCon*instance;
    if(!instance)
    {
        instance=new GroupCon();
    }
    return instance;
}

QListWidget *GroupCon::getGroupsList() const
{
    return groupsList;
}

void GroupCon::setGroupsList(QListWidget *newGroupsList)
{
    groupsList = newGroupsList;
}

QTextBrowser *GroupCon::getGroupTextBrow() const
{
    return groupTextBrow;
}

void GroupCon::setGroupTextBrow(QTextBrowser *newGroupTextBrow)
{
    groupTextBrow = newGroupTextBrow;
}

QLabel *GroupCon::getGroupNameLabel() const
{
    return groupNameLabel;
}

void GroupCon::setGroupNameLabel(QLabel *newGroupNameLabel)
{
    groupNameLabel = newGroupNameLabel;
}

QTextEdit *GroupCon::getGroupSendTextEdit() const
{
    return groupSendTextEdit;
}

void GroupCon::setGroupSendTextEdit(QTextEdit *newGroupSendTextEdit)
{
    groupSendTextEdit = newGroupSendTextEdit;
}

QListWidget *GroupCon::getGroupUsersList() const
{
    return groupUsersList;
}

void GroupCon::setGroupUsersList(QListWidget *newGroupUsersList)
{
    groupUsersList = newGroupUsersList;
}

QTextBrowser *GroupCon::getGroupDescBrow() const
{
    return groupDescBrow;
}

void GroupCon::setGroupDescBrow(QTextBrowser *newGroupDescBrow)
{
    groupDescBrow = newGroupDescBrow;
}


void GroupCon::onNewGroupMessageReceived(QString groupName)
{
    QString groupMsg;
    for(auto &val:_groupmodel.getCurrentMsg())
    {
//        if(val.groupName==groupName)
//        {
//            QString groupMsg;
//            for(auto &msg:val.groupMsg)
//            {
//                if(msg.first==AdminCon::getInstance()->getAdminName())
//                {
//                    groupMsg+=generateGroupMessageHTML(msg.first+":"+"<br>"+msg.second,true);
//                }
//                else
//                {
//                    groupMsg+=generateGroupMessageHTML(msg.first+":"+"<br>"+msg.second,false);
//                }
//            }
//            groupTextBrow->setHtml(groupMsg);
//            break;
//        }
        if(val.sendName==AdminCon::getInstance()->getAdminName())
        {
            groupMsg+=generateGroupMessageHTML(val.sendName+"<br>"+val.message,true);
        }
        else
        {
            groupMsg+=generateGroupMessageHTML(val.sendName+"<br>"+val.message,false);
        }
    }
    groupTextBrow->setHtml(groupMsg);

    QScrollBar*vScrollBar=groupTextBrow->verticalScrollBar();
    vScrollBar->setValue(vScrollBar->maximum());
}

void GroupCon::onNewGroupsReceived()
{
    QList<QString> list;
    for(auto temp : _groupmodel.getGroups())
    {
        list.push_back(temp.groupName);
    }
    emit showNewGroups(list);
}

void GroupCon::onGroupSelect(QListWidgetItem*groupName)
{
    bool temp = false;

    for(auto &group:_groupmodel.getGroups())
    {
        if(group.groupName==groupName->text())
        {
            groupNameLabel->setText(groupName->text());
            temp = true;
            _groupmodel.setCurrentGroup(group);
            qDebug()<<group.groupName<<"-"<<group.groupDesc;
            break;
        }
    }


    QString groupMsg;
    for(auto &msg:_groupmodel.groupMsgs[groupName->text()])
    {
        if(msg.sendName==AdminCon::getInstance()->getAdminName())
        {
            groupMsg+=generateGroupMessageHTML(msg.sendName+"<br>"+msg.message,true);
        }
        else
        {
            groupMsg+=generateGroupMessageHTML(msg.sendName+"<br>"+msg.message,false);
        }
    }
    groupTextBrow->setHtml(groupMsg);



    if(temp)
    {
        QList<QString> list;
        for(auto & user:_groupmodel.getCurrentGroup().groupUsers)
        {
            list.push_back(user.userName);
        }
        emit showNewUsers(list);
    }
    QString groupDesc;
    groupDesc+="群组姓名："+_groupmodel.getCurrentGroup().groupName+"\n";
    groupDesc+="群组描述："+_groupmodel.getCurrentGroup().groupDesc+"\n";
    groupDescBrow->setText(groupDesc);

    _groupmodel.setCurrentMsg(_groupmodel.groupMsgs[_groupmodel.getCurrentGroup().groupName]);
    QScrollBar*vScrollBar=groupTextBrow->verticalScrollBar();
    vScrollBar->setValue(vScrollBar->maximum());
}

void GroupCon::onGroupUserSelect(QListWidgetItem *userName)
{
    for(auto &val:_groupmodel.getGroups())
    {
        if(val.groupName==_groupmodel.getCurrentGroup().groupName)
        {
            for(auto &temp:val.groupUsers)
            {
                if(userName->text()==temp.userName)
                {
                    //qDebug()<<_groupmodel.getCurrentGroup().groupName<<":";
                    QString desc;
                    desc+="用户姓名："+temp.userName+"\n";
                    desc+="用户邮箱："+temp.userEmail+"\n";
                    desc+="用户手机："+temp.userPhone+"\n";
                    desc+="用户权限："+temp.userRole+"\n";
                    groupDescBrow->setText(desc);
                }
            }
        }
    }
}

void GroupCon::onSendBtn(bool cliecked)
{
    Q_UNUSED(cliecked)

    QString sendName = AdminCon::getInstance()->getAdminName();
    QString msg = groupSendTextEdit->toPlainText();
    _groupmodel.addMsg(sendName,msg);

    getGroupSendTextEdit()->clear();

    QJsonObject data;
    data["msgid"] = GROUP_CHAT_MSG;
    data["groupName"] = _groupmodel.getCurrentGroup().groupName;
    data["sendName"] = sendName;
    data["message"] = msg;
    QJsonArray recvName;
    for(auto val : _groupmodel.getCurrentGroup().groupUsers)
    {
        if(sendName==val.userName)
        {
            continue;
        }
        recvName.append(val.userName);
    }
    data["recvName"]=recvName;

    QJsonDocument doc(data);
    QByteArray json = doc.toJson();
    QSocket::getInstance().sendData(json);

    sendName=generateGroupMessageHTML(sendName,true);
    msg=generateGroupMessageHTML(msg,true);
    groupTextBrow->setHtml(groupTextBrow->toHtml()+sendName+msg);
    QScrollBar*vScrollBar=groupTextBrow->verticalScrollBar();
    vScrollBar->setValue(vScrollBar->maximum());
}
