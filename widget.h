#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QStackedWidget>
#include <QListWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDateTime>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVector>
#include <QString>
#include <QStringList>
#include <QCamera>
#include <QCameraInfo>
#include <QCameraViewfinder>
#include <QCameraImageCapture>

//历史记录每页最大数量
#define MAXCOUNTPERPAGE 8

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();


signals:
    void switch_accountsSignal();

private slots:
    void item_clicked(QListWidgetItem *item);
    void button2Add_clicked();
    void button2Departure_clicked();
    //page0
    void button0PageUp_clicked();
    void button0PageDown_clicked();
    //page3
    void button3PageUp_clicked();
    void button3PageDown_clicked();
    //page4
    void switch_accountsFunc();

private:
    Ui::Widget *ui;

    //menu控件
    QListWidget *listWidget_myList;
    //stack控件
    QStackedWidget *stackedWidget_myWidget;
    //组合menu和stack的layout
    QHBoxLayout *hBoxLayout_main;
    QGridLayout *gridLayout_main;
    //page0管理员控件
    QWidget *widget_Page0;
    QVBoxLayout *vBoxLayout0_Page0;
    QHBoxLayout *hBoxLayout0_Button;
    QSqlQueryModel *model_Administrators;
    QTableWidget *tableWidget_Administrators;
    QSqlDatabase db_Login;
    QPushButton *button0_PageUp;
    QPushButton *button0_PageDown;
    QLabel *label_AdministratorsCurAndAllPage;
    int per_AdministratorsCount;//每页内容数量
    int all_AdministratorsCount;//历史记录内容数量总计
    int cur_AdministratorsPage;//当前页数
    int all_AdministratorsPage;//总页数

    //page1控件
    QWidget *widget_Page1;
    QVBoxLayout *vBoxLayout_Page1;
    QVBoxLayout *vBoxLayout1_Info;
    QHBoxLayout *hBoxLayout1_IdentityCard;
    QHBoxLayout *hBoxLayout1_Name;
    QHBoxLayout *hBoxLayout1_Reason;
    QHBoxLayout *hBoxLayout1_Button;
    QLabel *lab1_IdentityCard;
    QLineEdit *le1_IdentityCard;
    QLabel *lab1_Name;
    QLineEdit *le1_Name;
    QLabel *lab1_Reason;
    QLineEdit *le1_Reason;
    QPushButton *button1_FaceRecognize;
    QPushButton *button1_Add;
    QPushButton *button1_Departure;

    //page2手动登记控件
    QWidget *widget_Page2;
    QVBoxLayout *vBoxLayout_Page2;
    QHBoxLayout *hBoxLayout2_Info;
    QHBoxLayout *hBoxLayout2_Button;
    QLabel *lab2_IdentityCard;
    QLineEdit *le2_IdentityCard;
    QLabel *lab2_Name;
    QLineEdit *le2_Name;
    QLabel *lab2_Reason;
    QLineEdit *le2_Reason;
    QPushButton *button2_Add;
    QPushButton *button2_Departure;

    //page3控件
    QWidget *widget_Page3;
    QVBoxLayout *vBoxLayout3_Page3;
    QHBoxLayout *hBoxLayout3_Button;
    QSqlQueryModel *model_History;
    QTableWidget *tableWidget_History;
    //QStringList *stringList_Title;
    QPushButton *button3_PageUp;
    QPushButton *button3_PageDown;
    QLabel *label_CurAndAllPage;
    int per_HistoryCount;//每页内容数量
    int all_HistoryCount;//历史记录内容数量总计
    int cur_HistoryPage;//当前页数
    int all_HistoryPage;//总页数

    //page4控件
    QWidget *widget_Page4;
    QVBoxLayout *vBoxLayout4_Page4;

    //page5
    QWidget *widget_page5;
    QVBoxLayout *vBoxLayout5_Page5;
    QHBoxLayout *hBoxLayout5_ID;
    QLabel *lab5_ID;
    QLineEdit *le5_ID;
    QHBoxLayout *hBoxLayout5_Name;
    QLabel *lab5_Name;
    QLineEdit *le5_Name;
    QHBoxLayout *hBoxLayout5_Button;
    QPushButton *button5_FaceEntry;
    QPushButton *button5_Save;


    //历史访客数据库
    QSqlDatabase db_visitorCheck;
    QSqlQuery query_main;
    QSqlRecord record_main;

    //时间变量
    QDateTime dateTime_current;

    void initControl();
    void initMenu();
    void initStacked();
    void initPage0();
    void initPage1();
    void initPage2();
    void initPage3();
    void reCheckSql();//重查数据库，并更新tableWidget，随时保证点击到page3时数据更新到最新
    void initPage4();
    void initPage5();
public:
    void reCheckSqlAdmin();//重查管理员数据库
};

#endif // WIDGET_H
