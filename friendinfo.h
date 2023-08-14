#ifndef FRIENDINFO_H
#define FRIENDINFO_H

#include <QDialog>
#include<QPoint>
#include<QMouseEvent>


namespace Ui {
class friendInfo;
}

class friendInfo : public QDialog
{
    Q_OBJECT

public:
    explicit friendInfo(QWidget *parent = nullptr);
    ~friendInfo();

    void mousePressEvent(QMouseEvent *event);//鼠标点击
    void mouseMoveEvent(QMouseEvent *event);//鼠标移动
    void mouseReleaseEvent(QMouseEvent *event);//鼠标释放

private slots:
    void on_closeBtn_clicked();

    void on_deleteFriendBtn_clicked();

private:
    Ui::friendInfo *ui;

    bool isPressedWidget;
    QPoint last;
};

#endif // FRIENDINFO_H
