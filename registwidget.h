#ifndef REGISTWIDGET_H
#define REGISTWIDGET_H

#include <QWidget>
#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

namespace Ui {
class RegistWidget;
}

class RegistWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RegistWidget(QWidget *parent = nullptr);
    ~RegistWidget();

signals:

private slots:
    void buttonOK_clicked();
    void buttonCancel_clicked();

private:
    Ui::RegistWidget *ui;

    QLabel *label_Account;
    QLabel *label_Password;
    QLabel *label_Administrator;

    QLineEdit *lineEdit_Account;
    QLineEdit *lineEdit_Password;
    QLineEdit *lineEdit_Administrator;

    QPushButton *button_OK;
    QPushButton *button_Cancel;

    QHBoxLayout *HBoxLayout_Account;
    QHBoxLayout *HBoxLayout_Password;
    QHBoxLayout *HBoxLayout_Administrator;
    QHBoxLayout *HBoxLayout_ocButton;

    QVBoxLayout *VBoxLayout_RegistMainLayout;

    QSqlDatabase db_Login;
    QSqlQuery query;

    int currentAccount;
    QString currentPassword;
};

#endif // REGISTWIDGET_H
