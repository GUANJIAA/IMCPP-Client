#ifndef ADDFRIENDDIA_H
#define ADDFRIENDDIA_H

#include <QDialog>
#include<QPoint>
#include<QMouseEvent>

namespace Ui {
class addFriendDia;
}

class addFriendDia : public QDialog
{
    Q_OBJECT

public:
    explicit addFriendDia(QWidget *parent = nullptr);
    ~addFriendDia();

    void mousePressEvent(QMouseEvent *event);//鼠标点击
    void mouseMoveEvent(QMouseEvent *event);//鼠标移动
    void mouseReleaseEvent(QMouseEvent *event);//鼠标释放

private slots:
    void on_toolButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::addFriendDia *ui;
    bool isPressedWidget;
    QPoint last;
};

#endif // ADDFRIENDDIA_H
