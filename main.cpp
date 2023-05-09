#include "widget.h"
#include "loginwidget.h"
#include <QApplication>
#include "base.h"

void aaa(base &b)
{

}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LoginWidget lgw;
    lgw.setLayout(lgw.VBoxLayout_loginMainLayout);
    lgw.show();

    base b;
    aaa(b);

    return a.exec();
}
