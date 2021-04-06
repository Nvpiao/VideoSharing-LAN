#ifndef QUDFLOATWIDGET_H
#define QUDFLOATWIDGET_H

#include <QWidget>
#include "qvslmainwindow.h"

class QUDFloatWidget : public QWidget {
    Q_OBJECT

private:

    QVSLMainWindow *mainWindow;

protected:

    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

public:

    explicit QUDFloatWidget(QWidget *parent = nullptr);
    void setMainWindow(QVSLMainWindow *mainWindow);
    void setVisiblily(QObject *parent,
                      bool     visible);
};

#endif // QUDFLOATWIDGET_H
