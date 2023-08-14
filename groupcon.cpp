#include "groupcon.h"

#include "public.h"
#include "admincon.h"

#include <QDebug>
#include "qsocket.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>

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
    // qDebug()<<groupName<<":"<<_groupmodel.getCurrentGroup().groupName;
    if(!(groupName==_groupmodel.getCurrentGroup().groupName))
    {
        // qDebug()<<"test";
        return ;
    }
    for(auto &val:_groupmodel.getGroupMsgs())
    {
        if(val.groupName==groupName)
        {
            QString groupMsg;
            for(auto &msg:val.groupMsg)
            {
                // qDebug()<<msg.first+":"+"<br>"+msg.second;
                if(msg.first==AdminCon::getInstance()->getAdminName())
                {
                    groupMsg+=generateGroupMessageHTML(msg.first+":"+"<br>"+msg.second,true);
                }
                else
                {
                    groupMsg+=generateGroupMessageHTML(msg.first+":"+"<br>"+msg.second,false);
                }
            }
            groupTextBrow->setHtml(groupMsg);
            break;
        }
    }
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
    for(auto &group:_groupmodel.getGroupMsgs())
    {
        if(group.groupName==groupName->text())
        {
            QString groupMsg;
            for(auto &msg:group.groupMsg)
            {
                if(msg.first==AdminCon::getInstance()->getAdminName())
                {
                    groupMsg+=generateGroupMessageHTML(msg.first+":"+"<br>"+msg.first,true);
                }
                else
                {
                    groupMsg+=generateGroupMessageHTML(msg.first+":"+"<br>"+msg.first,false);
                }
            }
            groupTextBrow->setHtml(groupMsg);
            break;
        }

    }
    bool temp = false;
    for(auto &group:_groupmodel.getGroups())
    {
        if(group.groupName==groupName->text())
        {
            groupNameLabel->setText(groupName->text());
            temp = true;
            _groupmodel.setCurrentGroup(group);
            //qDebug()<<group.groupName;
            break;
        }
    }

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
    // qDebug()<<data["groupName"]<<":"<<_groupmodel.getCurrentGroup().groupName;
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
}
