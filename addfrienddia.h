#ifndef ADDFRIENDDIA_H
#define ADDFRIENDDIA_H

#include <QDialog>

namespace Ui {
class addFriendDia;
}

class addFriendDia : public QDialog
{
    Q_OBJECT

public:
    explicit addFriendDia(QWidget *parent = nullptr);
    ~addFriendDia();

private slots:
    void on_toolButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::addFriendDia *ui;
};

#endif // ADDFRIENDDIA_H
