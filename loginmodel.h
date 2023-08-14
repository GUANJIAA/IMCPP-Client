#ifndef LOGINMODEL_H
#define LOGINMODEL_H

#include <QObject>
#include <QByteArray>

class LoginModel : public QObject
{
    Q_OBJECT
public:
    explicit LoginModel(QObject *parent = nullptr);

    static LoginModel*getInstance();

    void updateLoginData(const QByteArray &data);
    void updateRegisterData(const QByteArray &data);
    void updateRetrieveData(const QByteArray &data);
signals:
    void loginDataUpdated(const QByteArray &data);
    void registerDataUpdated(const QByteArray &data);
    void retrieveDataUpdated(const QByteArray &data);

private:
    QByteArray m_LoginData;
    QByteArray m_RegisterData;
    QByteArray m_RetrieveData;
};

#endif // LOGINMODEL_H
