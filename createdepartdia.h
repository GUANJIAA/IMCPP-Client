#ifndef CREATEDEPARTDIA_H
#define CREATEDEPARTDIA_H

#include <QDialog>

namespace Ui {
class CreateDepartdia;
}

class CreateDepartdia : public QDialog
{
    Q_OBJECT

public:
    explicit CreateDepartdia(QWidget *parent = nullptr);
    ~CreateDepartdia();

private slots:
    void on_createDepartBtn_clicked();

private:
    Ui::CreateDepartdia *ui;
};

#endif // CREATEDEPARTDIA_H
