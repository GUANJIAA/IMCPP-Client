#include "qsocket.h"

#include <QDebug>
#include <QByteArray>

QSocket::QSocket(QObject *parent)
: QObject{parent}
{
    QObject::connect(&m_sock,&QTcpSocket::readyRead,this,&QSocket::handleReadyRead);
}

bool QSocket::connectToServer(const QString &hostName, quint16 port)
{
    m_sock.disconnectFromHost();
    m_sock.waitForDisconnected();
    m_sock.connectToHost(hostName,port);
    return m_sock.waitForConnected();
}

void QSocket::sendData(const QByteArray &data)
{
    m_sock.write(data);
    m_sock.flush();
}

void QSocket::execServer()
{

}

void QSocket::handleReadyRead()
{
    QByteArray data=m_sock.readAll();
    emit dataReceived(data);
}


