#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("主界面");
    //this->resize();
    setAttribute(Qt::WA_DeleteOnClose);//构造函数里加这个，close是真正内存中删除

    /*****注册访客数据库*****/
    qDebug() << QSqlDatabase::drivers();
    db_visitor = QSqlDatabase::addDatabase("QSQLITE","connection_db_visitor");//创建QSqlite数据库连接,命名：数据库连接名connection_db_visitor
    db_visitor.setDatabaseName("visitor.db");//数据库名
    qDebug() << "创建了visitor.db";

    /*****打开访客数据库*****/
    if(db_visitor.open())//如果数据库成功打开
    {
        qDebug()<<"创建登录数据库成功！";
    }
    else
    {
        qDebug()<<db_visitor.lastError();
        exit(-1);
    }

    /*****创建表*****/
    query_main = QSqlQuery(db_visitor);
    query_main.exec("create table if not exists visitor_info(records integer primary key autoincrement not null,identityCard int,name text,accessTime text,departureTime text,reason text)");
    qDebug()<<"成功创建db_visitor表";

    /*****初始化控件*****/
    initControl();

    /*****构建UI菜单布局*****/
   gridLayout_main->addWidget(listWidget_myList,0,0);
   gridLayout_main->setColumnStretch(0,0);
   gridLayout_main->addWidget(stackedWidget_myWidget,0,1);
   gridLayout_main->setColumnStretch(1,3);

    /*****添加组件信息*****/
    initMenu();
    initPage0();
    initPage1();
    initPage2();
    initPage3();
    initPage4();
    initStacked();//必须放在所有init最后，否则显示不出来

    /*****将布局显示在上面*****/
    this->setLayout(gridLayout_main);


    /*****槽函数*****/
    connect(listWidget_myList,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(item_clicked(QListWidgetItem*)));
    //page2
    connect(button2_Add,SIGNAL(clicked()),this,SLOT(button2Add_clicked()));
    connect(button2_Departure,SIGNAL(clicked()),this,SLOT(button2Departure_clicked()));
    //page3
    //点击上一页
    connect(button3_PageUp,SIGNAL(clicked()),this,SLOT(button3PageUp_clicked()));
    //点击下一页
    connect(button3_PgeDown,SIGNAL(clicked()),this,SLOT(button3PageDown_clicked()));
}

Widget::~Widget()
{
    delete ui;
}

//new初始化控件
void Widget::initControl()
{

    /*****menu控件*****/
    listWidget_myList = new QListWidget;

    /*****stack控件*****/
    stackedWidget_myWidget = new QStackedWidget;

    /*****组合menu和stack的layout*****/
    hBoxLayout_main = new QHBoxLayout;
    gridLayout_main = new QGridLayout;

    /*****page0管理员控件*****/
    widget_Page0 = new QWidget;

    /*****page1控件*****/
    widget_Page1 = new QWidget;
    vBoxLayout_Page1 = new QVBoxLayout;
    vBoxLayout1_Info = new QVBoxLayout;
    hBoxLayout1_IdentityCard = new QHBoxLayout;
    hBoxLayout1_Name = new QHBoxLayout;
    hBoxLayout1_Reason = new QHBoxLayout;
    lab1_IdentityCard = new QLabel;
    le1_IdentityCard = new QLineEdit;
    lab1_Name = new QLabel;
    le1_Name = new QLineEdit;
    lab1_Reason = new QLabel;
    le1_Reason = new QLineEdit;

    /*****page2手动登记控件*****/
    widget_Page2 = new QWidget;
    vBoxLayout_Page2 = new QVBoxLayout;
    hBoxLayout2_Info = new QHBoxLayout;
    hBoxLayout2_Button = new QHBoxLayout;
    lab2_IdentityCard = new QLabel;
    le2_IdentityCard = new QLineEdit;
    lab2_Name = new QLabel;
    le2_Name = new QLineEdit;
    lab2_Reason = new QLabel;
    le2_Reason = new QLineEdit;
    button2_Add = new QPushButton;
    button2_Departure = new QPushButton;

    /*****page3访客历史记录*****/
    widget_Page3 = new QWidget;
    vBoxLayout3_Page3 = new QVBoxLayout;
    hBoxLayout3_Button = new QHBoxLayout;
    model_History = new QSqlQueryModel;
    tableWidget_History = new QTableWidget;
    button3_PageUp = new QPushButton;
    button3_PgeDown = new QPushButton;
    label_CurAndAllPage = new QLabel;
    all_HistoryCount = 0;
    per_HistoryCount = 0;
    cur_HistoryPage = 1;
    all_HistoryPage = 1;

    /*****page4*****/
    widget_Page4 = new QWidget;
}

void Widget::initMenu()
{
    listWidget_myList->addItem("管理员");
    listWidget_myList->addItem("人脸登记");
    listWidget_myList->addItem("手动登记");
    listWidget_myList->addItem("来访历史");
    listWidget_myList->addItem("切换账号");
    listWidget_myList->setFont(QFont("song", 14));
}

void Widget::initPage0()
{

}

void Widget::initPage1()
{
    //UI布局
    hBoxLayout1_IdentityCard->addWidget(lab1_IdentityCard);
    hBoxLayout1_IdentityCard->addWidget(le1_IdentityCard);
    hBoxLayout1_Name->addWidget(lab1_Name);
    hBoxLayout1_Name->addWidget(le1_Name);
    hBoxLayout1_Reason->addWidget(lab1_Reason);
    hBoxLayout1_Reason->addWidget(le1_Reason);

    vBoxLayout_Page1->addLayout(hBoxLayout1_IdentityCard);
    vBoxLayout_Page1->addLayout(hBoxLayout1_Name);
    vBoxLayout_Page1->addLayout(hBoxLayout1_Reason);

    widget_Page1->setLayout(vBoxLayout_Page1);

    //从数据库中调取
    lab1_IdentityCard->setText("身份证：");
    le1_IdentityCard->setText("需要修改");
    le1_IdentityCard->setReadOnly(true);

    lab1_Name->setText("姓  名：");
    le1_Name->setText("需要修改");
    le1_Name->setReadOnly(true);

    lab1_Reason->setText("原  因：");

}

void Widget::initPage2()
{
    //UI布局
    hBoxLayout2_Info->addWidget(lab2_IdentityCard);
    hBoxLayout2_Info->addWidget(le2_IdentityCard);
    hBoxLayout2_Info->addWidget(lab2_Name);
    hBoxLayout2_Info->addWidget(le2_Name);
    hBoxLayout2_Info->addWidget(lab2_Reason);
    hBoxLayout2_Info->addWidget(le2_Reason);

    hBoxLayout2_Button->addWidget(button2_Add);
    hBoxLayout2_Button->addWidget(button2_Departure);

    vBoxLayout_Page2->addLayout(hBoxLayout2_Info);
    vBoxLayout_Page2->addLayout(hBoxLayout2_Button);
    widget_Page2->setLayout(vBoxLayout_Page2);

    //添加UI信息
    lab2_Name->setText("姓名：");
    lab2_IdentityCard->setText("身份证：");
    lab2_Reason->setText("原因：");

    button2_Add->setText("访问");
    button2_Departure->setText("离开");
}

void Widget::initPage3()
{
    //UI布局
    hBoxLayout3_Button->addWidget(button3_PageUp);
    hBoxLayout3_Button->addWidget(button3_PgeDown);
    hBoxLayout3_Button->addWidget(label_CurAndAllPage);

    vBoxLayout3_Page3->addWidget(tableWidget_History);
    vBoxLayout3_Page3->addLayout(hBoxLayout3_Button);
    widget_Page3->setLayout(vBoxLayout3_Page3);
    widget_Page3->resize(QSize(700,500));
    tableWidget_History->resizeColumnsToContents();

    //添加UI信息
    button3_PageUp->setText("上一页");
    button3_PgeDown->setText("下一页");
    label_CurAndAllPage->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);//显示 “当前页数/总页数” 的label文本水平和竖直都居中

    //一定要指定数据库db_visitor，否则使用的是默认数据库，程序能执行，但会啥也查不出来！！
    model_History->setQuery("select identityCard,name,accessTime,departureTime,reason"
                            " from visitor_info order by records asc",db_visitor);


    //设置列数
    tableWidget_History->setColumnCount(5);
    //设置每列标题
    QStringList stringList_Title;
    stringList_Title<<"身份证"<<"姓名"<<"访问时间"<<"离开时间"<<"原因";
    tableWidget_History->setHorizontalHeaderLabels(stringList_Title);
    //设置不可编辑
    tableWidget_History->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //添加数据
    all_HistoryCount = model_History->rowCount();//数据总量
    per_HistoryCount = 12;//每页数据量
    if(all_HistoryCount % per_HistoryCount == 0)//计算总页数
    {
        all_HistoryPage = all_HistoryCount / per_HistoryCount;
    }
    else
    {
        all_HistoryPage = (all_HistoryCount / per_HistoryCount) + 1;
    }
    label_CurAndAllPage->setText(QString("%1/%2").arg(cur_HistoryPage).arg(all_HistoryPage));

    //第一页显示的数据
    for(int i = 0; i < 12; ++i)//行循环
    {
        int rowCount = tableWidget_History->rowCount();
        tableWidget_History->insertRow(rowCount);
        for(int j = 0; j < 5; ++j)//列循环
        {
            tableWidget_History->setItem(rowCount,j,new QTableWidgetItem(model_History->record(i).value(j).toString()));
        }
    }

    qDebug()<<"执行了！！！";
}

void Widget::reCheckSql()
{
    //一定要指定数据库db_visitor，否则使用的是默认数据库，程序能执行，但会啥也查不出来！！
    model_History->setQuery("select identityCard,name,accessTime,departureTime,reason"
                            " from visitor_info order by records asc",db_visitor);
    all_HistoryCount = model_History->rowCount();//数据总量
    per_HistoryCount = 12;//每页数据量
    if(all_HistoryCount % per_HistoryCount == 0)//计算总页数
    {
        all_HistoryPage = all_HistoryCount / per_HistoryCount;
    }
    else
    {
        all_HistoryPage = (all_HistoryCount / per_HistoryCount) + 1;
    }
    if(cur_HistoryPage > all_HistoryPage )//如果是最后一页
    {
        cur_HistoryPage = all_HistoryPage;
    }
    label_CurAndAllPage->setText(QString("%1/%2").arg(cur_HistoryPage).arg(all_HistoryPage));
    tableWidget_History->setRowCount(0);
    for(int i = 0+(cur_HistoryPage-1)*per_HistoryCount; i < 12+(cur_HistoryPage-1)*per_HistoryCount; ++i)//行循环
    {
        int rowCount = tableWidget_History->rowCount();
        tableWidget_History->insertRow(rowCount);
        for(int j = 0; j < 5; ++j)//列循环
        {
            tableWidget_History->setItem(rowCount,j,new QTableWidgetItem(model_History->record(i).value(j).toString()));
        }
    }
}

void Widget::initPage4()
{

}

void Widget::initStacked()
{
    stackedWidget_myWidget->addWidget(widget_Page0);
    stackedWidget_myWidget->addWidget(widget_Page1);
    stackedWidget_myWidget->addWidget(widget_Page2);
    stackedWidget_myWidget->addWidget(widget_Page3);
    stackedWidget_myWidget->addWidget(widget_Page4);
}


//槽函数

void Widget::item_clicked(QListWidgetItem *item)
{
    item = new QListWidgetItem;
    item = listWidget_myList->currentItem();
    int k = listWidget_myList->currentRow();
    switch (k) {
    case 0:
        stackedWidget_myWidget->setCurrentWidget(widget_Page0);
        qDebug()<<"stacked_page0 clicked!!!";
        break;
    case 1:
        stackedWidget_myWidget->setCurrentWidget(widget_Page1);
        qDebug()<<"stacked_page1 clicked!!!";
        break;
    case 2:
        stackedWidget_myWidget->setCurrentWidget(widget_Page2);
        qDebug()<<"stacked_page2 clicked!!!";
        break;
    case 3://来访记录更新后，要重新查一遍数据库,并更新tableWidget
        reCheckSql();
        stackedWidget_myWidget->setCurrentWidget(widget_Page3);
        qDebug()<<"stacked_page3 clicked!!!";
        break;
    case 4:
        switch_accountsFunc();
        qDebug()<<"stacked_page4 clicked!!!";
        break;
    default:
        qDebug()<<"error!!!";
        break;
    }

}

/*****page2*****/
void Widget::button2Add_clicked()
{
    int identityCard = le2_IdentityCard->text().toInt();
    QString name= le2_Name->text();
    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");

    //保证identityCard有信息
    if(le2_IdentityCard->text().isEmpty())
    {
        QMessageBox::information(this,"错误","请输入身份证信息！");
        return;
    }

    //如果正在访问，没有离开，则不可以再次点击访问
    QString str_insertVisitor = QString("select MAX(records),departureTime from visitor_info where identityCard = %1").arg(identityCard);
    if(!query_main.exec(str_insertVisitor)) //如果str_insertVisitor的SQL语句执行错误
    {
        qDebug()<<query_main.lastError().text();
        return;
    }

    //判断departureTime是否为空
    if(query_main.next())
    {
        /*
         * !!!!!!!!!!!!!!!!!!!!!!!!!1!问题!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
         * 此处由于sqlite用MAX函数查询没有的数据，会返回空值null，导致数据库内没有数据时候，无法添加数据，
         * 所以，要用!(query_main.value(0).isNull())保证使用sqlite数据库时，程序正常运行
         */
        if(!(query_main.value(0).isNull())&&query_main.value(1).isNull())//value(1)=departureTime
        {
            QMessageBox::information(this,"错误","您还未离开！");
            return;
        }
    }
    else
    {
        QMessageBox::information(this,"!!!","111");
    }


    str_insertVisitor = QString("insert into visitor_info(identityCard,name,accessTime) "
                                "values(%1,'%2','%3')").arg(identityCard).arg(name).arg(currentTime);
    if(!query_main.exec(str_insertVisitor)) //如果str_insertVisitor的SQL语句执行错误
    {
        qDebug()<<query_main.lastError().text();
        return;
    }
    QMessageBox::information(this,"成功","已登记");
}


void Widget::button2Departure_clicked()
{
    if(le2_IdentityCard->text().isEmpty())
    {
        QMessageBox::information(this,"错误","请输入身份证信息！");
        return;
    }
    int identityCard = le2_IdentityCard->text().toInt();
    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
    QString str_Departure = QString("update visitor_info set departureTime='%1' where records "
                                    "IN(select MAX(records) from visitor_info where identityCard=%2)").arg(currentTime).arg(identityCard);
    if(!query_main.exec(str_Departure)) //如果str_Departure的SQL语句执行错误
    {
        qDebug()<<query_main.lastError().text();
        return;
    }

    str_Departure = QString("select * from visitor_info where identityCard=%1").arg(identityCard);
    query_main.exec(str_Departure);
    if(query_main.next())//正确执行str_Departure,并找到identityCard
    {
        QMessageBox::information(this,"成功","已离开");
    }
    else
    {
        QMessageBox::information(this,"失败","未找到此人");
    }
}

/*****page3*****/
void Widget::button3PageUp_clicked()
{
    //tableWidget_History->clear();
    tableWidget_History->setRowCount(0); //设置行数
    if(all_HistoryCount<=per_HistoryCount)//只有一页
    {
        return;
    }
    qDebug()<<"last_Page";

    --cur_HistoryPage;
    if(cur_HistoryPage < 1 )//如果是第一页
    {
        cur_HistoryPage = 1;
    }
    for(int i = 0+(cur_HistoryPage-1)*per_HistoryCount; i < 12+(cur_HistoryPage-1)*per_HistoryCount; ++i)//行循环
    {
        int rowCount = tableWidget_History->rowCount();
        tableWidget_History->insertRow(rowCount);
        for(int j = 0; j < 5; ++j)//列循环
        {
            tableWidget_History->setItem(rowCount,j,new QTableWidgetItem(model_History->record(i).value(j).toString()));
        }
    }
    label_CurAndAllPage->setText(QString("%1/%2").arg(cur_HistoryPage).arg(all_HistoryPage));
}

void Widget::button3PageDown_clicked()
{
    tableWidget_History->setRowCount(0);
    if(all_HistoryCount <= per_HistoryCount)//只有一页
    {
        return;
    }
    qDebug()<<"next_Page";

    ++cur_HistoryPage;
    if(cur_HistoryPage > all_HistoryPage )//如果是最后一页
    {
        cur_HistoryPage = all_HistoryPage;
    }
    for(int i = 0+(cur_HistoryPage-1)*per_HistoryCount; i < 12+(cur_HistoryPage-1)*per_HistoryCount; ++i)//行循环
    {
        int rowCount = tableWidget_History->rowCount();
        tableWidget_History->insertRow(rowCount);
        for(int j = 0; j < 5; ++j)//列循环
        {
            tableWidget_History->setItem(rowCount,j,new QTableWidgetItem(model_History->record(i).value(j).toString()));
        }
    }
    label_CurAndAllPage->setText(QString("%1/%2").arg(cur_HistoryPage).arg(all_HistoryPage));
}

void Widget::switch_accountsFunc()
{
    QMessageBox msgBox;
    msgBox.setText("提示");
    msgBox.setInformativeText("确定要切换账号吗?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();
    if(ret == QMessageBox::Yes)
    {
        qDebug()<<"emit switch_accounts()";
        emit switch_accountsSignal();
    }
}
