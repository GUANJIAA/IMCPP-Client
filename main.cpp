#include "mainwindow.h"
#include "loginui.h"
#include "public.h"

#include <qsocket.h>
#include "logincon.h"
#include "dataprocessor.h"
#include "loginmodel.h"

#include <QChar>
#include <QDebug>
#include <thread>
#include <QTimer>
#include <QObject>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTcpSocket>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSocket::getInstance().connectToServer("192.168.61.100",9000);

    DataProcessor dataProcessor;

    LoginUI* login = new LoginUI();
    MainWindow* mainwindow = new MainWindow();

    QObject::connect(login,&LoginUI::switchToMainWindows,[&](){
        mainwindow->show();
    });

    login->show();

    LoginCon::getInstance();

    // w.show();
    return a.exec();
}
