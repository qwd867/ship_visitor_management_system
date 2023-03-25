#include "registwidget.h"
#include "ui_registwidget.h"

RegistWidget::RegistWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegistWidget)
{
    ui->setupUi(this);
    this->setWindowTitle("注册");

    //组件初始化
    label_Account = new QLabel;
    lineEdit_Account = new QLineEdit;
    label_Password = new QLabel;
    lineEdit_Password = new QLineEdit;
    label_Administrator = new QLabel;
    lineEdit_Administrator = new QLineEdit;

    button_OK = new QPushButton;
    button_Cancel = new QPushButton;

    HBoxLayout_Account = new QHBoxLayout;
    HBoxLayout_Password = new QHBoxLayout;
    HBoxLayout_Administrator = new QHBoxLayout;
    HBoxLayout_ocButton = new QHBoxLayout;

    VBoxLayout_RegistMainLayout = new QVBoxLayout;


    //添加组件信息
    label_Account->setText("账号: ");
    label_Password->setText("密码: ");
    label_Administrator->setText("管理员密码:");
    button_OK->setText("确定");
    button_Cancel->setText("取消");


    //构建UI布局
    HBoxLayout_Account->addWidget(label_Account);
    HBoxLayout_Account->addWidget(lineEdit_Account);

    HBoxLayout_Password->addWidget(label_Password);
    HBoxLayout_Password->addWidget(lineEdit_Password);

    HBoxLayout_Administrator->addWidget(label_Administrator);
    HBoxLayout_Administrator->addWidget(lineEdit_Administrator);

    HBoxLayout_ocButton->addWidget(button_OK);
    HBoxLayout_ocButton->addWidget(button_Cancel);

    VBoxLayout_RegistMainLayout->addLayout(HBoxLayout_Account);
    VBoxLayout_RegistMainLayout->addLayout(HBoxLayout_Password);
    VBoxLayout_RegistMainLayout->addLayout(HBoxLayout_Administrator);
    VBoxLayout_RegistMainLayout->addLayout(HBoxLayout_ocButton);

    //将布局显示在上面
    this->setLayout(VBoxLayout_RegistMainLayout);

    //注册管理员数据库
    qDebug() << QSqlDatabase::drivers();
    db_Login = QSqlDatabase::addDatabase("QSQLITE");//创建QSqlite数据库连接
    /*db_Login.setHostName("localhost");//主机服务器
    db_Login.setPort(3306);//端口
    db_Login.setUserName("root");//用户名
    db_Login.setPassword("062888");//密码*/
    db_Login.setDatabaseName("login.db");//数据库名
    qDebug() << "创建了login.db";

    //打开管理员数据库
    if(db_Login.open())//如果数据库成功打开
    {
        qDebug()<<"创建登录数据库成功！";
    }
    else
    {
        qDebug()<<db_Login.lastError();
        exit(-1);
    }

    //信号与槽
    connect(button_OK,SIGNAL(clicked()),this,SLOT(buttonOK_clicked()));
    connect(button_Cancel,SIGNAL(clicked()),this,SLOT(buttonCancel_clicked()));
}

RegistWidget::~RegistWidget()
{
    delete ui;
}

void RegistWidget::buttonOK_clicked()
{
    if(lineEdit_Account->text().isEmpty()||lineEdit_Password->text().isEmpty())
    {
        QMessageBox::information(this,"错误","请输入账号密码！");
        return;
    }
    if(lineEdit_Administrator->text() != "123")
    {
        QMessageBox::information(this,"错误","管理员密码错误！");
        return;
    }

    //检查数据库是否存在相同账号
    currentAccount = lineEdit_Account->text().toInt();
    currentPassword = lineEdit_Password->text();

    query = QSqlQuery(db_Login);//本语句必须放在此处，不能放在构造函数处，否则会出错
    QString str_Account = QString("select * from login_info where account_id=%1").arg(currentAccount);
    if(!query.exec(str_Account)) //SQL语句执行错误
    {
        qDebug()<<query.lastError().text();
        return;
    }


    if(query.next())//如果str_account的SQL语句执行正确，并查到相应信息
    {
        QMessageBox::information(this,"错误","账号已存在");
    }
    else//如果str_account的SQL语句执行正确，并没有查到相应信息
    {
        QString str_insert=QString("insert into login_info(account_id,password) values(%1,'%2')").arg(currentAccount).arg(currentPassword);
        if(!query.exec(str_insert)) //如果str_insert的SQL语句执行错误
        {
            qDebug()<<query.lastError().text();
            return;
        }
        QMessageBox::information(this,"成功","创建成功");
        this->close();
    }

}

void RegistWidget::buttonCancel_clicked()
{
    this->close();
}
