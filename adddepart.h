#ifndef ADDDEPART_H
#define ADDDEPART_H

#include <QDialog>
#include<QPoint>
#include<QMouseEvent>

namespace Ui {
class AddDepart;
}

class AddDepart : public QDialog
{
    Q_OBJECT

public:
    explicit AddDepart(QWidget *parent = nullptr);
    ~AddDepart();

    void mousePressEvent(QMouseEvent *event);//鼠标点击
    void mouseMoveEvent(QMouseEvent *event);//鼠标移动
    void mouseReleaseEvent(QMouseEvent *event);//鼠标释放

private slots:
    void on_addDepartBtn_clicked();

    void on_toolButton_clicked();

private:
    Ui::AddDepart *ui;

    bool isPressedWidget;
    QPoint last;
};

#endif // ADDDEPART_H
