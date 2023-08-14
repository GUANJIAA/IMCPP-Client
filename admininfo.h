#ifndef ADMININFO_H
#define ADMININFO_H

#include <QDialog>
#include<QPoint>
#include<QMouseEvent>

namespace Ui {
class adminInfo;
}

class adminInfo : public QDialog
{
    Q_OBJECT

public:
    explicit adminInfo(QWidget *parent = nullptr);
    ~adminInfo();

    void mousePressEvent(QMouseEvent *event);//鼠标点击
    void mouseMoveEvent(QMouseEvent *event);//鼠标移动
    void mouseReleaseEvent(QMouseEvent *event);//鼠标释放

private slots:
    void on_toolButton_2_clicked();

    void on_commitBtn_clicked();

    void onCommitResult(QString msg);

private:
    Ui::adminInfo *ui;

    bool isPressedWidget;
    QPoint last;
};

#endif // ADMININFO_H
