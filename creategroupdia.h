#ifndef CREATEGROUPDIA_H
#define CREATEGROUPDIA_H

#include <QDialog>

namespace Ui {
class createGroupDia;
}

class createGroupDia : public QDialog
{
    Q_OBJECT

public:
    explicit createGroupDia(QWidget *parent = nullptr);
    ~createGroupDia();

private slots:
    void on_toolButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::createGroupDia *ui;
};

#endif // CREATEGROUPDIA_H
