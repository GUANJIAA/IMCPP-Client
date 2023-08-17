#ifndef DEPARTINFO_H
#define DEPARTINFO_H

#include <QDialog>
#include<QPoint>
#include<QMouseEvent>

namespace Ui {
class DepartInfo;
}

class DepartInfo : public QDialog
{
    Q_OBJECT

public:
    explicit DepartInfo(QWidget *parent = nullptr);
    ~DepartInfo();

    void mousePressEvent(QMouseEvent *event);//鼠标点击
    void mouseMoveEvent(QMouseEvent *event);//鼠标移动
    void mouseReleaseEvent(QMouseEvent *event);//鼠标释放

private slots:
    void on_quitDepartBtn_clicked();

    void on_toolButton_clicked();

private:
    Ui::DepartInfo *ui;
    bool isPressedWidget;
    QPoint last;
};

#endif // DEPARTINFO_H
