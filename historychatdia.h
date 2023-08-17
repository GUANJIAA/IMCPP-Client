#ifndef HISTORYCHATDIA_H
#define HISTORYCHATDIA_H

#include <QDialog>

namespace Ui {
class historyChatDia;
}

class historyChatDia : public QDialog
{
    Q_OBJECT

public:
    explicit historyChatDia(QWidget *parent = nullptr);
    ~historyChatDia();

private:
    Ui::historyChatDia *ui;
};

#endif // HISTORYCHATDIA_H
