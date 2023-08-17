#ifndef HISTORYCHATMSGDIA_H
#define HISTORYCHATMSGDIA_H

#include <QDialog>
#include <QPushButton>
#include<QPoint>
#include<QMouseEvent>

namespace Ui {
class historyChatMsgDia;
}

class historyChatMsgDia : public QDialog
{
    Q_OBJECT

public:
    explicit historyChatMsgDia(QWidget *parent = nullptr);
    ~historyChatMsgDia();

    void mousePressEvent(QMouseEvent *event);//鼠标点击
    void mouseMoveEvent(QMouseEvent *event);//鼠标移动
    void mouseReleaseEvent(QMouseEvent *event);//鼠标释放

    void setData(std::vector<int> vec,QStringList sendNames,QStringList messages);

    QPushButton *getDeleteMsgBtn() const;
    void setDeleteMsgBtn(QPushButton *newDeleteMsgBtn);

    QString getChat() const;
    void setChat(const QString &newChat);

public slots:
    //void onRecvHistoryMsg(bool result);

private slots:

    void on_deleteChatMsg_clicked();

    void on_addChatMsg_clicked();

    void on_toolButton_clicked();

private:
    Ui::historyChatMsgDia *ui;
    std::vector<int> msgidVec;
    QPushButton*deleteMsgBtn;
    QString chat="Friend";
    bool isPressedWidget;
    QPoint last;
};

#endif // HISTORYCHATMSGDIA_H
