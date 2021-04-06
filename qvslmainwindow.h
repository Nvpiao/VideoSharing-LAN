#ifndef QVSLMAINWINDOW_H
#define QVSLMAINWINDOW_H

#include <QMainWindow>

#include <QMediaPlayer>
#include <QFileDialog>
#include <QMouseEvent>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class QVSLMainWindow; }
QT_END_NAMESPACE

class QVSLMainWindow : public QMainWindow {
    Q_OBJECT

private:

    bool isPlaylistShownState = false; // invisible
    bool isMaxState = false;

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
    void keyPressEvent(QKeyEvent *event);

public:

    QVSLMainWindow(QWidget *parent = nullptr);
    ~QVSLMainWindow();

    bool getPlaylistShownState();

    void setPlayListWidgetVisable();
    void setTopAndBottomWidgetVisiable(bool visiable);

    void mousePress(QMouseEvent *event);
    void mouseRelease(QMouseEvent *event,
                      bool         clickFlag);
    void mouseMove(QMouseEvent *event);
    void keyPress(QKeyEvent *event);

    void setPlayButtonIconPlay();
    void setPlayButtonIconPause();

private slots:

    void on_durationChanged(qint64 duration);
    void on_positionChanged(qint64 position);
    void on_currentIndexChanged(int position);
    void on_error(QMediaPlayer::Error error);

    void on_btnList_clicked();
    void on_btnMax_clicked();
    void on_btnFullScreen_clicked();
    void on_btnAdds_clicked();
    void on_btnDelete_clicked();
    void on_listVideoWidget_doubleClicked(const QModelIndex& index);
    void on_btnPlay_clicked();
    void on_btnStop_clicked();
    void on_btnClear_clicked();
    void on_btnSkipBack_clicked();
    void on_btnSkipForward_clicked();
    void on_btnRewind10_clicked();
    void on_btnForward10_clicked();
    void on_sliderVolumn_valueChanged(int value);
    void on_sliderProgress_valueChanged(int value);
    void on_btnVolumn_clicked();
    void on_btnOpen_clicked();
    void on_btnRefresh_clicked();

private:

    Ui::QVSLMainWindow *ui;

    bool static moreThan(int a,
                         int b); // comparing function
    void        setVideoInfoEmpty();
    void        initButtonState();
    void        setPlayButtonIcon(QIcon icon);
    bool        isVideoListEmpty();
    void        videoFullScreen();
    void        videoPlayAndPause();
};
#endif // QVSLMAINWINDOW_H
