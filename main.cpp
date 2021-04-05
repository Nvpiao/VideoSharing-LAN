#include "qvslmainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QVSLMainWindow w;
    w.show();
    return a.exec();
}
