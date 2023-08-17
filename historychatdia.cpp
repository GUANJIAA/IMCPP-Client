#include "historychatdia.h"
#include "ui_historychatdia.h"

historyChatDia::historyChatDia(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::historyChatDia)
{
    ui->setupUi(this);
}

historyChatDia::~historyChatDia()
{
    delete ui;
}
