#include "qudvideowidget.h"

void QUDVideoWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (mainWindow->isFullScreen()) {
            mainWindow->showNormal();
            mainWindow->setTopAndBottomWidgetVisiable(true);
        } else {
            mainWindow->showFullScreen();
            mainWindow->setTopAndBottomWidgetVisiable(false);
        }

        // if state of playlist is true, change the state of playListWidget.
        if (mainWindow->getPlaylistShownState()) {
            mainWindow->setPlayListWidgetVisable();
        }
    }

    QVideoWidget::mouseDoubleClickEvent(event);
}

void QUDVideoWidget::mousePressEvent(QMouseEvent *event)
{
    timer->start(100);
    clickFlag = true;
    mainWindow->mousePress(event);
    QVideoWidget::mousePressEvent(event);
}

void QUDVideoWidget::mouseReleaseEvent(QMouseEvent *event)
{
    timer->stop();
    mainWindow->mouseRelease(event, clickFlag);
    QVideoWidget::mouseReleaseEvent(event);
}

void QUDVideoWidget::mouseMoveEvent(QMouseEvent *event)
{
    mainWindow->mouseMove(event);
    QVideoWidget::mouseMoveEvent(event);
}

QUDVideoWidget::QUDVideoWidget(QWidget *parent)
    : QVideoWidget(parent)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(on_timeout()));
}

void QUDVideoWidget::setPlayer(QMediaPlayer *player, QVSLMainWindow *mainWindow)
{
    this->player = player;
    this->mainWindow = mainWindow;
}

void QUDVideoWidget::on_timeout()
{
    timer->stop();
    clickFlag = false;
}
