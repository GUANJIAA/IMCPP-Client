#include "departcon.h"

#include "admincon.h"
#include "qsocket.h"
#include "public.h"
#include "admincon.h"

#include <QDebug>
#include <QScrollBar>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>

QString generateDepartMsgHTML(const QString&text,bool isSendByte)
{
    QString alignment = isSendByte ? "right" : "left";
    return "<div style=\"text-align: " + alignment + "; width: 100%;\">" + text + "</div>";
}

DepartCon::DepartCon(QObject *parent)
    : QObject{parent}
{
    connect(&_departmodel,&DepartModel::newDepart,this,&DepartCon::onNewDepartUsersReceived);
    connect(&_departmodel,&DepartModel::newDepartMsg,this,&DepartCon::onNewDepartMessageReceived);
    connect(&_departmodel,&DepartModel::recvHistoryMsg,this,&DepartCon::onRecvHistroyMsg);
}

DepartCon *DepartCon::getInstance()
{
    static DepartCon*instance;
    if(!instance)
    {
        instance = new DepartCon();
    }
    return instance;
}

QListWidget *DepartCon::getDepartUsersList() const
{
    return departUsersList;
}

void DepartCon::setDepartUsersList(QListWidget *newDepartUsersList)
{
    departUsersList = newDepartUsersList;
}

QTextBrowser *DepartCon::getDepartTextBrow() const
{
    return departTextBrow;
}

void DepartCon::setDepartTextBrow(QTextBrowser *newDepartTextBrow)
{
    departTextBrow = newDepartTextBrow;
}

QTextEdit *DepartCon::getDepartSendTextEdit() const
{
    return departSendTextEdit;
}

void DepartCon::setDepartSendTextEdit(QTextEdit *newDepartSendTextEdit)
{
    departSendTextEdit = newDepartSendTextEdit;
}

QTextBrowser *DepartCon::getDepartDescTextBrow() const
{
    return departDescTextBrow;
}

void DepartCon::setDepartDescTextBrow(QTextBrowser *newDepartDescTextBrow)
{
    departDescTextBrow = newDepartDescTextBrow;
}

QLabel *DepartCon::getDepartNameLabel() const
{
    return departNameLabel;
}

void DepartCon::setDepartNameLabel(QLabel *newDepartNameLabel)
{
    departNameLabel = newDepartNameLabel;
}

void DepartCon::onNewDepartMessageReceived()
{
    // qDebug()<<"----";
    QString departMsg;
    for(auto &msg:_departmodel.getDepartMsgs())
    {
        // qDebug()<<"----";
        if(msg.sendName==AdminCon::getInstance()->getAdminName())
        {
            departMsg+=generateDepartMsgHTML(msg.sendName+"<br>"+msg.msg,true);
        }
        else
        {
            departMsg+=generateDepartMsgHTML(msg.sendName+"<br>"+msg.msg,false);
        }
    }
    departTextBrow->setHtml(departMsg);

}

void DepartCon::onNewDepartUsersReceived()
{
    departNameLabel->setText(AdminCon::getInstance()->getAdminDepartName());
    QList<QString> list;
    for(auto temp:_departmodel.getCurrentDepart().departUsers)
    {
        list.push_back(temp.userName);
    }
    emit showNewDepartUsers(list);
}

void DepartCon::onRecvHistroyMsg()
{
    QString msg;
    for(auto &val:_departmodel.getDepartMsgs())
    {
        if(val.sendName==AdminCon::getInstance()->getAdminName())
        {
            msg+=generateDepartMsgHTML(val.sendName+"<br>"+val.msg,true);
        }
        else if(val.sendName!=AdminCon::getInstance()->getAdminName())
        {
            msg+=generateDepartMsgHTML(val.sendName+"<br>"+val.msg,false);
        }
    }
    departTextBrow->setHtml(msg);

    QScrollBar*vScrollBar=departTextBrow->verticalScrollBar();
    vScrollBar->setValue(vScrollBar->maximum());
}

void DepartCon::onSendBtn(bool cliecked)
{
    Q_UNUSED(cliecked)

    if(AdminCon::getInstance()->getAdminDepartName()=="TEXT")
    {
        return ;
    }

    QString sendName = AdminCon::getInstance()->getAdminName();
    QString msg = departSendTextEdit->toPlainText();
    _departmodel.addDepartMsg(sendName,msg);

    getDepartSendTextEdit()->clear();

    QJsonObject data;
    data["msgid"] = DEPART_CHAT_MSG;
    data["departName"] = _departmodel.getCurrentDepart().departName;
    data["sendName"] = sendName;
    data["message"] = msg;
    QJsonArray recvName;
    for(auto val : _departmodel.getCurrentDepart().departUsers)
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

    sendName=generateDepartMsgHTML(sendName,true);
    msg=generateDepartMsgHTML(msg,true);
    departTextBrow->setHtml(departTextBrow->toHtml()+sendName+msg);
}

void DepartCon::onDepartUserSelect(QListWidgetItem*item)
{
    QString userName=item->text();
    for(auto&val:_departmodel.getCurrentDepart().departUsers)
    {
        if(userName==val.userName)
        {
            QString userInfo;
            userInfo+="用户姓名："+val.userName+"\n";
            userInfo+="用户邮箱："+val.userEmail+"\n";
            userInfo+="用户手机："+val.userPhone+"\n";
            userInfo+="用户权限："+val.userRole+"\n";
            departDescTextBrow->setText(userInfo);
            break;
        }
    }
}
