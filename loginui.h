#ifndef LOGINUI_H
#define LOGINUI_H


#include <QMenu>
#include <QPoint>
#include <QMouseEvent>
#include <QSystemTrayIcon>

namespace Ui {
class LoginUI;
}

class LoginUI : public QWidget
{
    Q_OBJECT

public:
    explicit LoginUI(QWidget *parent = nullptr);
    ~LoginUI();


    void mousePressEvent(QMouseEvent *event);//鼠标点击
    void mouseMoveEvent(QMouseEvent *event);//鼠标移动
    void mouseReleaseEvent(QMouseEvent *event);//鼠标释放

signals:
    void switchToMainWindows();

private slots:

    void showwidget();
    void closewidget();

    void on_CloseBtn_clicked();

    void on_LoginBtn_clicked();

    void on_RegisterBtn_clicked();

    void on_RetrieveBtn_clicked();

    void onLoginSuccess();
    void onLoginFailed(QString msg);

private:

    Ui::LoginUI *ui;
    bool isPressedWidget;
    QPoint last;
};

#endif // LOGINUI_H
