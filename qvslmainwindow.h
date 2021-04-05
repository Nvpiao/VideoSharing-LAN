#ifndef QVSLMAINWINDOW_H
#define QVSLMAINWINDOW_H

#include <QMainWindow>

#include <QMediaPlayer>
#include <QFileDialog>
#include <QMouseEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class QVSLMainWindow; }
QT_END_NAMESPACE

class QVSLMainWindow : public QMainWindow {
    Q_OBJECT

private:

    bool isPlaylistShownState = false; // invisible

    bool isDrag = false;
    QPoint startPos;
    QPoint windowTopLeftPos;

    QMediaPlayer *player;
    QMediaPlaylist *playlist;

    QString currentTime;
    QString totalTime;

protected:

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

public:

    QVSLMainWindow(QWidget *parent = nullptr);
    ~QVSLMainWindow();

    bool                getPlaylistShownState();

    Ui::QVSLMainWindow* getUi();
    void                setPlayListWidgetVisable();
    void                setTopAndBottomWidgetVisiable(bool visiable);

    void                mousePress(QMouseEvent *event);
    void                mouseRelease(QMouseEvent *event);
    void                mouseMove(QMouseEvent *event);

private slots:

    void on_stateChanged(QMediaPlayer::State state);
    void on_durationChanged(qint64 duration);
    void on_positionChanged(qint64 position);
    void on_currentIndexChanged(int position);

    void on_btnList_clicked();
    void on_btnMax_clicked();
    void on_btnFullScreen_clicked();
    void on_btnAdds_clicked();

private:

    Ui::QVSLMainWindow *ui;
};
#endif // QVSLMAINWINDOW_H
