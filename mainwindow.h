#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QString>
#include <QMainWindow>
#include <QPoint>
#include <QMouseEvent>
#include <QMenu>
#include <QSystemTrayIcon>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void mousePressEvent(QMouseEvent *event);//鼠标点击
    void mouseMoveEvent(QMouseEvent *event);//鼠标移动
    void mouseReleaseEvent(QMouseEvent *event);//鼠标释放

signals:
    void switchToWidget();
private slots:
    void updateAdminData();
    void showFriendsList(const QList<QString>&friends);
    void showGroupsList(const QList<QString>&groups);
    void showGroupUsers(const QList<QString>&users);
    void showDepartUsers(const QList<QString>&users);
private slots:

    void on_CloseBtn_clicked();

    void on_MinBtn_clicked();

    void on_MaxBtn_clicked();

    void on_InfoBtn_clicked();

    void on_FriendBtn_clicked();

    void on_IconBtn_clicked();

    void on_DepartBtn_clicked();

    void on_GroupBtn_clicked();

    void showwidget();
    void closewidget();

    void on_friendMenuBtn_clicked();

    void on_reviseBtn_clicked();

    void on_MenuBtn_clicked();

    void on_friendFlushBtn_clicked();

    void on_friendAddBtn_clicked();

    void on_historyChatBtn_clicked();

    void on_groupMenuBtn_clicked();

    void on_groupFlushBtn_clicked();

    void on_addGroupBtn_clicked();

    void on_createGroupBtn_clicked();

    void on_departFlushBtn_clicked();

    void on_groupHistoryBtn_clicked();

private:
    Ui::MainWindow *ui;

    QSystemTrayIcon  * systemtrayicon;  //系统托盘
    QMenu * menu;
    QAction *m_pShowAction;
    QAction *m_pCloseAction;


    bool isPressedWidget;
    QPoint last;
};
#endif // MAINWINDOW_H
