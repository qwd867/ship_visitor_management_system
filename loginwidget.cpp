#include "loginwidget.h"
#include "ui_loginwidget.h"

LoginWidget::LoginWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWidget)
{
    ui->setupUi(this);
    this->setWindowTitle("登录");

    //组件初始化
    label_Account = new QLabel;
    lineEdit_Account = new QLineEdit;
    label_Password = new QLabel;
    lineEdit_Password = new QLineEdit;

    button_Login = new QPushButton;
    button_Regist = new QPushButton;

    HBoxLayout_Account = new QHBoxLayout;
    HBoxLayout_Password = new QHBoxLayout;
    HBoxLayout_lrcButton = new QHBoxLayout;

    VBoxLayout_loginMainLayout = new QVBoxLayout;

    w = new Widget;
    rgw = new RegistWidget;


    //添加组件信息
    label_Account->setText("账号: ");
    lineEdit_Account->setPlaceholderText("请输入账号");
    label_Password->setText("密码: ");
    lineEdit_Password->setPlaceholderText("请输入密码");
    lineEdit_Password->setEchoMode(QLineEdit::Password);

    button_Login->setText("登录");
    button_Regist->setText("注册");


    //构建UI布局
    HBoxLayout_Account->addWidget(label_Account);
    HBoxLayout_Account->addWidget(lineEdit_Account);

    HBoxLayout_Password->addWidget(label_Password);
    HBoxLayout_Password->addWidget(lineEdit_Password);

    HBoxLayout_lrcButton->addWidget(button_Login);
    HBoxLayout_lrcButton->addWidget(button_Regist);

    VBoxLayout_loginMainLayout->addLayout(HBoxLayout_Account);
    VBoxLayout_loginMainLayout->addLayout(HBoxLayout_Password);
    VBoxLayout_loginMainLayout->addLayout(HBoxLayout_lrcButton);



    //注册管理员数据库
    qDebug() << QSqlDatabase::drivers();
    db_Login = QSqlDatabase::addDatabase("QSQLITE","connection_db_login");//创建QSqlite数据库连接，命名：数据库连接名connection_db_login
    db_Login.setDatabaseName("login.db");//数据库名
    qDebug() << "设置登录数据库名login.db";

    //打开管理员数据库
    if(db_Login.open())//如果数据库成功打开
    {
        qDebug()<<"打开登录数据库成功！";
    }
    else
    {
        qDebug()<<db_Login.lastError();
        exit(-1);
    }

    //创建表
    query = QSqlQuery(db_Login);
    query.exec("create table if not exists login_info(account_id integer primary key autoincrement,password text)");
    qDebug()<<"成功创建db_login表";

    //信号与槽
    connect(button_Login,SIGNAL(clicked()),this,SLOT(buttonLogin_clicked()));
    connect(button_Regist,SIGNAL(clicked()),this,SLOT(buttonRegist_clicked()));
    connect(w,SIGNAL(switch_accountsSignal()),this,SLOT(switch_accountsLgFunc()));
}

LoginWidget::~LoginWidget()
{
    delete ui;
}

void LoginWidget::buttonLogin_clicked()
{
    //重查管理人员信息数据库，防止注册后再登录查询不到刚才注册的账号
    w->reCheckSqlAdmin();

    if(lineEdit_Account->text().isEmpty()||lineEdit_Password->text().isEmpty())
    {
        QMessageBox::information(this,"登录","账号或密码为空！");
        return;
    }

    currentAccount = lineEdit_Account->text().toInt();
    currentPassword = lineEdit_Password->text();

    QString str_Account = QString("select * from login_info where account_id=%1").arg(currentAccount);
    QString str_Password = QString("select * from login_info where account_id=%1 and password='%2'").arg(currentAccount).arg(currentPassword);

    //账号是否存在
    if(!query.exec(str_Account)) //SQL语句执行错误
    {
        qDebug()<<query.lastError().text();
        return;
    }
    if(!query.next())//SQL语句执行正确，但没有查询到账号信息
    {
        QMessageBox::information(this,"登录","账号不存在！");
        return;
    }

    //密码是否正确
    if(!query.exec(str_Password)) //SQL语句执行错误
    {
        qDebug()<<query.lastError().text();
        return;
    }
    if(query.next())//SQL语句执行正确，并查到相应信息
    {
        //QMessageBox::information(this,"登录","登录成功！");
        this->hide();
        w->show();
    }
    else//SQL语句执行正确，并没有查到相应信息
    {
        QMessageBox::information(this,"登录","密码错误！");
    }
}

void LoginWidget::buttonRegist_clicked()
{
    rgw->show();
}

void LoginWidget::switch_accountsLgFunc()
{
    qDebug()<<"swtichAccount";
    this->show();

    w->close();//这里加了setAttribute(Qt::WA_DeleteOnClose);，真正在内存中删除了w
    w=new Widget;//新建一个w
    connect(w,SIGNAL(switch_accountsSignal()),this,SLOT(switch_accountsLgFunc()));//和新建的w建立信号与槽
}
