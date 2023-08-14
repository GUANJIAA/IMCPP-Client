#ifndef RETRIEVEUI_H
#define RETRIEVEUI_H

#include <QDialog>
#include <QPoint>
#include <QMouseEvent>

namespace Ui {
class retrieveui;
}

class retrieveui : public QDialog
{
    Q_OBJECT

public:
    explicit retrieveui(QWidget *parent = nullptr);
    ~retrieveui();

    void mousePressEvent(QMouseEvent *event);//鼠标点击
    void mouseMoveEvent(QMouseEvent *event);//鼠标移动
    void mouseReleaseEvent(QMouseEvent *event);//鼠标释放

private slots:
    void on_CloseBtn_clicked();

    void onRetrieveSuccess(QString msg);
    void onRetrieveFailed(QString msg);

    void on_retrieveBtn_clicked();

private:
    Ui::retrieveui *ui;

    bool isPressedWidget;
    QPoint last;
};

#endif // RETRIEVEUI_H
