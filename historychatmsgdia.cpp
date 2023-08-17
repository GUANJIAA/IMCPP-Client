#include "historychatmsgdia.h"
#include "ui_historychatmsgdia.h"

#include "friendcon.h"
#include "friendmodel.h"
#include "admincon.h"

#include "public.h"
#include "qsocket.h"

#include <QTableView>
#include <QHeaderView>
#include <QStringList>
#include <QStandardItemModel>
#include <QString>
#include <QStringList>
#include <QPushButton>
#include <QRadioButton>
#include <QCheckBox>
#include <QMessageBox>
#include <QDebug>
#include <unistd.h>
#include <QTableWidget>
#include <QTableWidgetItem>

#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>

historyChatMsgDia::historyChatMsgDia(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::historyChatMsgDia)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::SplashScreen|Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);

    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setFont(QFont("宋体",16));
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"发送者"<<"消息体"<<"是否删除");
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    setDeleteMsgBtn(ui->deleteChatMsg);
}

historyChatMsgDia::~historyChatMsgDia()
{
    delete ui;
}

void historyChatMsgDia::mousePressEvent(QMouseEvent *event)
{
    isPressedWidget = true; // 当前鼠标按下的即是QWidget而非界面上布局的其它控件
    last = event->globalPos();
}

void historyChatMsgDia::mouseMoveEvent(QMouseEvent *event)
{
    if (isPressedWidget)
    {
        int dx = event->globalX() - last.x();
        int dy = event->globalY() - last.y();
        last = event->globalPos();
        move(x()+dx, y()+dy);
    }
}

void historyChatMsgDia::mouseReleaseEvent(QMouseEvent *event)
{
    int dx = event->globalX() - last.x();
    int dy = event->globalY() - last.y();
    move(x()+dx, y()+dy);
    isPressedWidget = false; // 鼠标松开时，置为false
}

void historyChatMsgDia::setData(std::vector<int> vec,QStringList sendNames, QStringList messages)
{
    for(int i=0;i<sendNames.size();++i)
    {

        this->ui->tableWidget->insertRow(i);
        QTableWidgetItem*sendName=new QTableWidgetItem(sendNames[i]);
        sendName->setTextAlignment(Qt::AlignCenter);
        this->ui->tableWidget->setItem(i,0,sendName);

        QTableWidgetItem*message=new QTableWidgetItem(messages[i]);
        message->setTextAlignment(Qt::AlignCenter);
        this->ui->tableWidget->setItem(i,1,message);

        QWidget*cellWidget=new QWidget();
        QCheckBox *box = new QCheckBox(cellWidget);
        this->ui->tableWidget->setCellWidget(i,2,cellWidget);

        if(vec.size()==sendNames.size())
        {
            msgidVec.push_back(vec[i]);
        }

    }
}

//void historyChatMsgDia::onRecvHistoryMsg(bool result)
//{
//    if(result)
//    {
//        this->close();
//    }
//    else
//    {
//        QMessageBox::information(this,"Message","add history message failed");
//    }
//}

void historyChatMsgDia::on_deleteChatMsg_clicked()
{
    for(int r=0;r<this->ui->tableWidget->rowCount();++r)
    {
        QWidget*cellWidget=this->ui->tableWidget->cellWidget(r,2);
        QCheckBox*checkBox=cellWidget->findChild<QCheckBox*>();
        if(checkBox&&checkBox->isChecked())
        {
            //qDebug()<<r;
            QJsonObject data;
            data["msgid"]=DEL_CHAT_MSG;
            data["msgId"]=msgidVec[r];
            msgidVec.erase(msgidVec.begin()+r);
            QString sendName = this->ui->tableWidget->item(r,0)->text();
            QString msg = this->ui->tableWidget->item(r,1)->text();
            QString recvName;
            if(sendName==AdminCon::getInstance()->getAdminName())
            {
                recvName=FriendCon::getInstance()->currentFriend.userName;
            }
            else
            {
                recvName=AdminCon::getInstance()->getAdminName();
            }
            data["sendName"]=sendName;
            data["recvName"]=recvName;
            data["message"]=msg;

            QJsonDocument doc(data);
            QByteArray json = doc.toJson();
//            qDebug()<<json;
            QSocket::getInstance().sendData(json);

            this->ui->tableWidget->removeRow(r);
            --r;
        }
    }
}

void historyChatMsgDia::on_addChatMsg_clicked()
{
    if(chat=="Friend")
    {
        QJsonObject data;
        data["msgid"] = QUERY_CHAT_MSG;
        data["recvName"] = AdminCon::getInstance()->getAdminName();
        data["sendName"] = FriendCon::getInstance()->currentFriend.userName;

        QJsonDocument doc(data);
        QByteArray json = doc.toJson();
        QSocket::getInstance().sendData(json);
    }
    else if(chat=="Group")
    {
        QJsonObject data;
        data["msgid"] = QUERY_GROUPCHAT_MSG;
        data["userName"]=AdminCon::getInstance()->getAdminName();

        QJsonDocument doc(data);
        QByteArray json = doc.toJson();
        QSocket::getInstance().sendData(json);
    }
    else if(chat=="Depart")
    {
        QJsonObject data;
        data["msgid"] = QUERY_DEPARTCHAT_MSG;   // 查询部门聊天信息;
        data["userName"] = AdminCon::getInstance()->getAdminName();

        QJsonDocument doc(data);
        QByteArray json = doc.toJson();
        QSocket::getInstance().sendData(json);
        //qDebug()<<json;
    }

    this->close();
}

QString historyChatMsgDia::getChat() const
{
    return chat;
}

void historyChatMsgDia::setChat(const QString &newChat)
{
    chat = newChat;
}

QPushButton *historyChatMsgDia::getDeleteMsgBtn() const
{
    return deleteMsgBtn;
}

void historyChatMsgDia::setDeleteMsgBtn(QPushButton *newDeleteMsgBtn)
{
    deleteMsgBtn = newDeleteMsgBtn;
}


void historyChatMsgDia::on_toolButton_clicked()
{
    this->close();
}

