#ifndef ADMINCON_H
#define ADMINCON_H

#include <QObject>
#include <QString>

class AdminCon : public QObject
{
    Q_OBJECT
public:
    explicit AdminCon(QObject *parent = nullptr);

    static AdminCon*getInstance();

    QString getAdminName() const;
    void setAdminName(const QString &newAdminName);

    QString getAdminPwd() const;
    void setAdminPwd(const QString &newAdminPwd);

    QString getAdminEmail() const;
    void setAdminEmail(const QString &newAdminEmail);

    QString getAdminPhone() const;
    void setAdminPhone(const QString &newAdminPhone);

    QString getAdminDesc() const;
    void setAdminDesc(const QString &newAdminDesc);

    QByteArray getData() const;
    void setData(const QByteArray &newData);

    void updateData(QByteArray data);

    void reviseAdminResult(QByteArray data);

    QString getAdminDepartName() const;
    void setAdminDepartName(const QString &newAdminDepartName);

signals:
    void updateAdmin();

    void reviseAdmin(QString msg);

private:
    QByteArray data;

    QString adminName;
    QString adminPwd;
    QString adminEmail;
    QString adminPhone;
    QString adminDesc;
    QString adminDepartName;
};

#endif // ADMINCON_H
