#ifndef LOGINCON_H
#define LOGINCON_H

#include "loginmodel.h"

#include <QWidget>
#include <QObject>
#include <QPushButton>

class LoginCon : public QObject
{
    Q_OBJECT
public:
    explicit LoginCon(QObject *parent = nullptr);

    static LoginCon&getInstance();

signals:
    void loginSeccess();
    void loginFailed(QString msg);
    void registerSuccess(QString msg);
    void registerFailed(QString msg);
    void retrieveSuccess(QString msg);
    void retrieveFailed(QString msg);

public slots:
    void updateLoginData(const QByteArray &data);
    void updateRegisterData(const QByteArray &data);
    void updateRetrieveData(const QByteArray &data);

private:
    LoginModel* m_model;
};

#endif // LOGINCON_H
