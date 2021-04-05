#include "qvslmainwindow.h"

#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QDesktopWidget *pDesk = QApplication::desktop();
    QVSLMainWindow  w;

    w.move((pDesk->width() - w.width()) / 2, (pDesk->height() - w.height()) / 2);
    qDebug() << pDesk->width() << w.width() << pDesk->height() << w.height();
    w.show();
    return a.exec();
}
