#ifndef GROUPINFODIA_H
#define GROUPINFODIA_H

#include <QDialog>
#include<QPoint>
#include<QMouseEvent>

namespace Ui {
class GroupInfoDia;
}

class GroupInfoDia : public QDialog
{
    Q_OBJECT

public:
    explicit GroupInfoDia(QWidget *parent = nullptr);
    ~GroupInfoDia();

    void mousePressEvent(QMouseEvent *event);//鼠标点击
    void mouseMoveEvent(QMouseEvent *event);//鼠标移动
    void mouseReleaseEvent(QMouseEvent *event);//鼠标释放

private slots:
    void on_quitGroupBtn_clicked();

    void on_toolButton_clicked();

private:
    Ui::GroupInfoDia *ui;
    bool isPressedWidget;
    QPoint last;
};

#endif // GROUPINFODIA_H
