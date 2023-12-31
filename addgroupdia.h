#ifndef ADDGROUPDIA_H
#define ADDGROUPDIA_H

#include <QDialog>
#include<QPoint>
#include<QMouseEvent>

namespace Ui {
class addGroupDia;
}

class addGroupDia : public QDialog
{
    Q_OBJECT

public:
    explicit addGroupDia(QWidget *parent = nullptr);
    ~addGroupDia();

    void mousePressEvent(QMouseEvent *event);//鼠标点击
    void mouseMoveEvent(QMouseEvent *event);//鼠标移动
    void mouseReleaseEvent(QMouseEvent *event);//鼠标释放

private slots:
    void on_pushButton_clicked();

    void on_toolButton_clicked();

private:
    Ui::addGroupDia *ui;
    bool isPressedWidget;
    QPoint last;
};

#endif // ADDGROUPDIA_H
