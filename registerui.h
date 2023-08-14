#ifndef REGISTERUI_H
#define REGISTERUI_H

#include <QDialog>
#include <QPoint>
#include <QMouseEvent>

namespace Ui {
class registerui;
}

class registerui : public QDialog
{
    Q_OBJECT

public:
    explicit registerui(QWidget *parent = nullptr);
    ~registerui();

    void mousePressEvent(QMouseEvent *event);//鼠标点击
    void mouseMoveEvent(QMouseEvent *event);//鼠标移动
    void mouseReleaseEvent(QMouseEvent *event);//鼠标释放

public slots:
    void onRegisterSuccessed(QString msg);
    void onRegisterFailed(QString msg);

private slots:
    void on_CloseBtn_clicked();

    void on_registerBtn_clicked();

private:
    Ui::registerui *ui;

    bool isPressedWidget;
    QPoint last;
};

#endif // REGISTERUI_H
