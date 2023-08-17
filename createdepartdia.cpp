#include "createdepartdia.h"
#include "ui_createdepartdia.h"

#include "admincon.h"
#include "public.h"
#include "qsocket.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>

CreateDepartdia::CreateDepartdia(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateDepartdia)
{
    ui->setupUi(this);
}

CreateDepartdia::~CreateDepartdia()
{
    delete ui;
}

void CreateDepartdia::on_createDepartBtn_clicked()
{
    QJsonObject data;
    data["msgid"] = CREATE_DEPART_MSG;
    // data["userName"] = AdminCon::getInstance()->getAdminName();
    data["departName"] = ui->departNameLineEdit->text();
    data["departDesc"] = ui->deprtdescLineEdit->text();

    QJsonDocument doc(data);
    QByteArray json = doc.toJson();
    QSocket::getInstance().sendData(json);

//    QJsonObject temp;
//    temp["msgid"]=ADD_DEPART_MSG;


    this->close();
}

