#ifndef QSOCKET_H
#define QSOCKET_H

#include <QObject>
#include <QTcpSocket>

class QSocket : public QObject
{
    Q_OBJECT
public:
    explicit QSocket(QObject *parent = nullptr);

    static QSocket&getInstance()
    {
        static QSocket sock;
        return sock;
    }

    bool connectToServer(const QString &hostName,quint16 port);

    void sendData(const QByteArray &data);

    void execServer();

signals:
    void dataReceived(const QByteArray&data);

private slots:
    void handleReadyRead();

private:
    QTcpSocket m_sock;
};

#endif // QSOCKET_H
