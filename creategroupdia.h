#ifndef CREATEGROUPDIA_H
#define CREATEGROUPDIA_H

#include <QDialog>
#include<QPoint>
#include<QMouseEvent>

namespace Ui {
class createGroupDia;
}

class createGroupDia : public QDialog
{
    Q_OBJECT

public:
    explicit createGroupDia(QWidget *parent = nullptr);
    ~createGroupDia();

    void mousePressEvent(QMouseEvent *event);//鼠标点击
    void mouseMoveEvent(QMouseEvent *event);//鼠标移动
    void mouseReleaseEvent(QMouseEvent *event);//鼠标释放

private slots:
    void on_toolButton_2_clicked();

    void on_createGroupBtn_clicked();

private:
    Ui::createGroupDia *ui;
    bool isPressedWidget;
    QPoint last;
};

#endif // CREATEGROUPDIA_H
