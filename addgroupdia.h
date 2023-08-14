#ifndef ADDGROUPDIA_H
#define ADDGROUPDIA_H

#include <QDialog>

namespace Ui {
class addGroupDia;
}

class addGroupDia : public QDialog
{
    Q_OBJECT

public:
    explicit addGroupDia(QWidget *parent = nullptr);
    ~addGroupDia();

private slots:
    void on_pushButton_clicked();

    void on_toolButton_clicked();

private:
    Ui::addGroupDia *ui;
};

#endif // ADDGROUPDIA_H
