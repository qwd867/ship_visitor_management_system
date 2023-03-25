#include "widget.h"
#include "loginwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LoginWidget lgw;
    lgw.setLayout(lgw.VBoxLayout_loginMainLayout);
    lgw.show();

    return a.exec();
}
