#include <QApplication>
#include "snowflakes.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Snowflakes snowflakes(&a);
    snowflakes.showMaximized();
    snowflakes.startSnowfall();
    return a.exec();
}
