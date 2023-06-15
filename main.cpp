#include <QApplication>
#include <QFont>
#include "mainui.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFont font("Microsoft YaHei", 9);
    a.setFont(font);

    MainUI w;
    w.show();

    return a.exec();
}
