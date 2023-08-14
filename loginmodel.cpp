#include "loginmodel.h"

#include <QDebug>

LoginModel::LoginModel(QObject *parent)
    : QObject{parent}{}

LoginModel *LoginModel::getInstance()
{
    static LoginModel*instance;
    if(!instance)
    {
        instance = new LoginModel();
    }
    return instance;
}

void LoginModel::updateLoginData(const QByteArray &data)
{
    m_LoginData = data;
    emit loginDataUpdated(m_LoginData);
}

void LoginModel::updateRegisterData(const QByteArray &data)
{
    m_RegisterData = data;
    emit registerDataUpdated(m_RegisterData);
}

void LoginModel::updateRetrieveData(const QByteArray &data)
{
    m_RetrieveData = data;
    emit retrieveDataUpdated(m_RetrieveData);
}
