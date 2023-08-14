#ifndef DEPARTCON_H
#define DEPARTCON_H

#include "departmodel.h"

#include <QObject>
#include <QListWidget>
#include <QTextBrowser>
#include <QLabel>
#include <QTextEdit>

class DepartCon : public QObject
{
    Q_OBJECT
public:
    explicit DepartCon(QObject *parent = nullptr);

    static DepartCon*getInstance();

signals:


public:
    DepartModel _departmodel;

    QListWidget *getDepartUsersList() const;
    void setDepartUsersList(QListWidget *newDepartUsersList);

    QTextBrowser *getDepartTextBrow() const;
    void setDepartTextBrow(QTextBrowser *newDepartTextBrow);

    QTextEdit *getDepartSendTextEdit() const;
    void setDepartSendTextEdit(QTextEdit *newDepartSendTextEdit);

    QTextBrowser *getDepartDescTextBrow() const;
    void setDepartDescTextBrow(QTextBrowser *newDepartDescTextBrow);

    QLabel *getDepartNameLabel() const;
    void setDepartNameLabel(QLabel *newDepartNameLabel);

signals:
    void showNewDepartUsers(const QList<QString>&departUsers);

public slots:
    void onNewDepartMessageReceived();
    void onNewDepartUsersReceived();

    void onSendBtn(bool cliecked);
    void onDepartUserSelect(QListWidgetItem*item);

private:
    QListWidget* departUsersList;
    QTextBrowser*departTextBrow;
    QLabel *departNameLabel;
    QTextEdit*departSendTextEdit;
    QTextBrowser*departDescTextBrow;
};

#endif // DEPARTCON_H
