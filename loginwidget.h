#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QDebug>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QMessageBox>
#include "widget.h"
#include "registwidget.h"

namespace Ui {
class LoginWidget;
}

class LoginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = nullptr);
    ~LoginWidget();
    QVBoxLayout *VBoxLayout_loginMainLayout;

signals:

private slots:
    void buttonLogin_clicked();
    void buttonRegist_clicked();
    void switch_accountsLgFunc();

private:
    Ui::LoginWidget *ui;
    friend class RegistWidget;

    QLabel *label_Account;
    QLabel *label_Password;

    QLineEdit *lineEdit_Account;
    QLineEdit *lineEdit_Password;

    QPushButton *button_Login;
    QPushButton *button_Regist;

    QHBoxLayout *HBoxLayout_Account;
    QHBoxLayout *HBoxLayout_Password;
    QHBoxLayout *HBoxLayout_lrcButton;

    Widget *w;//主页面对象指针
    RegistWidget *rgw;//注册页面指针

    QSqlDatabase db_Login;
    QSqlQuery query;

    int currentAccount;
    QString currentPassword;
};

#endif // LOGINWIDGET_H
