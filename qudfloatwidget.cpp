#include "qudfloatwidget.h"

#include <QLayout>

void QUDFloatWidget::enterEvent(QEvent *event)
{
    if (mainWindow->isFullScreen()) {
        this->setVisiblily(this, true);
    }
    QWidget::enterEvent(event);
}

void QUDFloatWidget::leaveEvent(QEvent *event)
{
    if (mainWindow->isFullScreen()) {
        this->setVisiblily(this, false);
    }
    QWidget::leaveEvent(event);
}

void QUDFloatWidget::setVisiblily(QObject *parent, bool visible) {
    if (parent->inherits(QBoxLayout::staticMetaObject.className())) {
        QBoxLayout *layout = static_cast<QBoxLayout *>(parent);

        QObjectList kidLayouts = layout->children();

        if (kidLayouts.isEmpty()) {
            for (int i = 0; i < layout->count(); ++i) {
                QWidget *widget = qobject_cast<QWidget *>
                                      (layout->itemAt(i)->widget());

                if (widget != Q_NULLPTR) {
                    this->setVisiblily(widget, visible);
                }
            }
        } else {
            foreach(QObject * kid, kidLayouts) {
                this->setVisiblily(kid, visible);
            }
        }
    } else {
        QObjectList kids = parent->children();

        if (kids.isEmpty()) {
            static_cast<QWidget *>(parent)->setVisible(visible);
        } else {
            foreach(QObject * kid, kids) {
                this->setVisiblily(kid, visible);
            }
        }
    }
}

QUDFloatWidget::QUDFloatWidget(QWidget *parent) : QWidget(parent)
{}

void QUDFloatWidget::setMainWindow(QVSLMainWindow *mainWindow)
{
    this->mainWindow = mainWindow;
}
