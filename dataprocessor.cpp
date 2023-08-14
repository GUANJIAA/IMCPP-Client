#include "dataprocessor.h"

#include "public.h"
#include "qsocket.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

DataProcessor::DataProcessor(QObject *parent)
    : QObject{parent}
{
    connect(&QSocket::getInstance(),&QSocket::dataReceived,this,&DataProcessor::processData);
}

void DataProcessor::processData(const QByteArray &data)
{
    QByteArray delimiter = "@@@";
    QByteArray temp = data;

    while(true)
    {
        int delimitPos = temp.indexOf(delimiter);
        if(delimitPos==-1)
        {
            break;
        }
        QByteArray recvData = temp.mid(0,delimitPos);
        temp=temp.mid(delimitPos+delimiter.length());
        // qDebug()<<recvData;
        QJsonDocument doc = QJsonDocument::fromJson(recvData);
        QJsonObject obj = doc.object();
        if(obj["msgid"].toInt()==LOGIN_MSG)
        {
            LoginModel::getInstance()->updateLoginData(recvData);
        }
        else if(obj["msgid"].toInt()==REGISTER_MSG)
        {
            LoginModel::getInstance()->updateRegisterData(recvData);
        }
        else if(obj["msgid"].toInt()==RETRIEVE_MSG)
        {
            LoginModel::getInstance()->updateRetrieveData(recvData);
        }
        else if(obj["msgid"].toInt()==NEW_CONNECTION)
        {
            AdminCon::getInstance()->updateData(recvData);
        }
        else if(obj["msgid"].toInt()==QUERY_ADMININFO)
        {
            AdminCon::getInstance()->updateData(recvData);
        }
        else if(obj["msgid"].toInt()==UPDATE_ADMININFO)
        {
            AdminCon::getInstance()->reviseAdminResult(recvData);
        }
        else if(obj["msgid"].toInt()==QUERY_FRIEND_MSG)
        {
            FriendCon::getInstance()->_friendmodel.recvNewFriends(recvData);
        }
        else if(obj["msgid"].toInt()==ONE_CHAT_MSG)
        {
            FriendCon::getInstance()->_friendmodel.recvNewMsg(recvData);
        }
        else if(obj["msgid"].toInt()==QUERY_CHAT_MSG)
        {
            FriendCon::getInstance()->_friendmodel.queryChatMsg(recvData);
        }
        else if(obj["msgid"].toInt()==QUERY_OFFLINE_CHAT_MSG)
        {
            FriendCon::getInstance()->_friendmodel.recvNewMsg(recvData);
        }
        else if(obj["msgid"].toInt()==GROUP_CHAT_MSG)
        {
            GroupCon::getInstance()->_groupmodel.recvNewMsg(recvData);
        }
        else if(obj["msgid"].toInt()==QUERY_GROUP_MSG)
        {
            GroupCon::getInstance()->_groupmodel.recvNewGroups(recvData);
        }
        else if(obj["msgid"].toInt()==QUERY_DEPART_USERS_MSG)
        {
            DepartCon::getInstance()->_departmodel.recvNewDepart(recvData);
        }
        else if(obj["msgid"].toInt()==DEPART_CHAT_MSG)
        {
            DepartCon::getInstance()->_departmodel.recvNewDepartMsg(recvData);
        }
        else if(obj["msgid"].toInt()==ADD_DEPART_MSG)
        {

        }
    }

}
