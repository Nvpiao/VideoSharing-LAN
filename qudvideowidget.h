#ifndef QUDVIDEOWIDGET_H
#define QUDVIDEOWIDGET_H

#include <QVideoWidget>
#include <QMediaPlayer>
#include <QKeyEvent>
#include <QMediaPlaylist>
#include <QTimer>

#include "qvslmainwindow.h"

class QUDVideoWidget : public QVideoWidget {
    Q_OBJECT

private:

    QMediaPlayer *player = Q_NULLPTR;
    QMediaPlaylist *playlist = Q_NULLPTR;

    bool isDrag = false;
    QPoint startPos;
    QPoint windowTopLeftPos;

    QTimer *timer;
    bool clickFlag = false;

    QVSLMainWindow *mainWindow;

protected:

    void mouseDoubleClickEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

public:

    QUDVideoWidget(QWidget *parent = Q_NULLPTR);

    void setPlayer(QMediaPlayer   *player,
                   QVSLMainWindow *mainWindow);

private slots:

    void on_timeout();
};

#endif // QUDVIDEOWIDGET_H
