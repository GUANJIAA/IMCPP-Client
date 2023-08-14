#ifndef GROUPCON_H
#define GROUPCON_H

#include "groupmodel.h"

#include <QObject>
#include <QMap>
#include <QList>
#include <QLabel>
#include <QString>
#include <QTextEdit>
#include <QStringList>
#include <QListWidget>
#include <QTextBrowser>

class GroupCon : public QObject
{
    Q_OBJECT
public:
    explicit GroupCon(QObject *parent = nullptr);

    static GroupCon*getInstance();

    QListWidget *getGroupsList() const;
    void setGroupsList(QListWidget *newGroupsList);

    QTextBrowser *getGroupTextBrow() const;
    void setGroupTextBrow(QTextBrowser *newGroupTextBrow);

    QLabel *getGroupNameLabel() const;
    void setGroupNameLabel(QLabel *newGroupNameLabel);

    QTextEdit *getGroupSendTextEdit() const;
    void setGroupSendTextEdit(QTextEdit *newGroupSendTextEdit);

    QListWidget *getGroupUsersList() const;
    void setGroupUsersList(QListWidget *newGroupUsersList);

    QTextBrowser *getGroupDescBrow() const;
    void setGroupDescBrow(QTextBrowser *newGroupDescBrow);

signals:
    void showNewGroups(const QList<QString>&friends);
    void showNewUsers(const QList<QString>&friends);

public slots:
    void onNewGroupMessageReceived(QString groupName);
    void onNewGroupsReceived();

    void onGroupSelect(QListWidgetItem*groupName);
    void onGroupUserSelect(QListWidgetItem*userName);
    void onSendBtn(bool cliecked);

public:
    GroupModel _groupmodel;

private:
    QListWidget* groupsList;
    QTextBrowser* groupTextBrow;
    QLabel* groupNameLabel;
    QTextEdit* groupSendTextEdit;
    QListWidget* groupUsersList;
    QTextBrowser* groupDescBrow;



};

#endif // GROUPCON_H
