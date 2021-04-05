#include "qvslmainwindow.h"
#include "ui_qvslmainwindow.h"

void QVSLMainWindow::mousePressEvent(QMouseEvent *event)
{
    this->mousePress(event);
    QWidget::mousePressEvent(event);
}

void QVSLMainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    this->mouseRelease(event);
    QWidget::mouseReleaseEvent(event);
}

void QVSLMainWindow::mouseMoveEvent(QMouseEvent *event)
{
    this->mouseMove(event);
    QWidget::mouseMoveEvent(event);
}

QVSLMainWindow::QVSLMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::QVSLMainWindow)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::FramelessWindowHint);
    ui->listVideoWidget->setFrameShape(QListWidget::NoFrame);
    ui->listVideoWidget->setStyleSheet("background-color:rgb(60,60,60)");
    ui->playListWidget->setVisible(false);

    player = new QMediaPlayer(this);
    player->setNotifyInterval(2000);
    player->setVideoOutput(ui->widgetVideo);
    ui->widgetVideo->setPlayer(player, this);

    //    ui->widgetVideo->setAspectRatioMode(Qt::KeepAspectRatio);

    playlist = new QMediaPlaylist(this);
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    player->setPlaylist(playlist);

    connect(player,   SIGNAL(stateChanged(QMediaPlayer::State)),
            this, SLOT(on_stateChanged(QMediaPlayer::State)));
    connect(player,   SIGNAL(durationChanged(qint64)),
            this, SLOT(on_durationChanged(qint64)));
    connect(player,   SIGNAL(positionChanged(qint64)),
            this, SLOT(on_positionChanged(qint64)));
    connect(playlist, SIGNAL(currentIndexChanged(int)),
            this, SLOT(on_currentIndexChanged(int)));
}

QVSLMainWindow::~QVSLMainWindow()
{
    delete ui;
}

bool QVSLMainWindow::getPlaylistShownState()
{
    return this->isPlaylistShownState;
}

Ui::QVSLMainWindow * QVSLMainWindow::getUi()
{
    return this->ui;
}

void QVSLMainWindow::on_btnList_clicked()
{
    ui->playListWidget->setVisible(!isPlaylistShownState);
    isPlaylistShownState = !isPlaylistShownState;
}

void QVSLMainWindow::on_btnMax_clicked()
{}

void QVSLMainWindow::on_btnFullScreen_clicked()
{
    if (this->isFullScreen()) {
        this->showNormal();
        this->setTopAndBottomWidgetVisiable(true);
    } else {
        this->showFullScreen();
        this->setTopAndBottomWidgetVisiable(false);
    }

    // if state of playlist is true, change the state of playListWidget.
    if (isPlaylistShownState) {
        this->setPlayListWidgetVisable();
    }
}

void QVSLMainWindow::setTopAndBottomWidgetVisiable(bool visiable) {
    ui->widgetBottom->setVisible(visiable);
    ui->widgetTop->setVisible(visiable);
}

void QVSLMainWindow::mousePress(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        isDrag = true;
        startPos = event->globalPos();
        windowTopLeftPos = frameGeometry().topLeft();
    }
}

void QVSLMainWindow::mouseRelease(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        isDrag = false;
    }
}

void QVSLMainWindow::mouseMove(QMouseEvent *event)
{
    if (isDrag && !isFullScreen()) {
        QPoint dis = event->globalPos() - startPos;
        this->move(dis + windowTopLeftPos);
    }
}

void QVSLMainWindow::on_stateChanged(QMediaPlayer::State state)
{
    ui->btnPlay->setEnabled(state != QMediaPlayer::PlayingState);
    ui->btnPause->setEnabled(state == QMediaPlayer::PlayingState);
    ui->btnStop->setEnabled(state == QMediaPlayer::PlayingState);
    ui->btnForward10->setEnabled(state == QMediaPlayer::PlayingState);
    ui->btnRewind10->setEnabled(state == QMediaPlayer::PlayingState);
}

void QVSLMainWindow::on_durationChanged(qint64 duration)
{
    ui->sliderProgress->setMaximum(duration);
    qint64 secs = duration / 1000;
    qint64 hours = secs / 3600;
    qint64 mins = (secs - hours * 3600) / 60;

    secs %= 60;
    currentTime = QString::asprintf("%d:%d:%d", (int)hours, (int)mins, (int)secs);
    ui->labDuration->setText(currentTime + '/' + totalTime);
}

void QVSLMainWindow::on_positionChanged(qint64 position)
{
    if (ui->sliderProgress->isSliderDown()) {
        return;
    }

    ui->sliderProgress->setSliderPosition(position);
    qint64 secs = position / 1000;
    qint64 hours = secs / 3600;
    qint64 mins = (secs - hours * 3600) / 60;

    secs %= 60;
    totalTime = QString::asprintf("%d:%d:%d", (int)hours, (int)mins, (int)secs);
    ui->labDuration->setText(currentTime + '/' + totalTime);
}

void QVSLMainWindow::on_currentIndexChanged(int position)
{
    ui->listVideoWidget->setCurrentRow(position);

    QListWidgetItem *curItem = ui->listVideoWidget->item(position);
    QString videoName = curItem->toolTip();
    int     lastDot = videoName.lastIndexOf('.');

    // set video name and type
    ui->labVideoType->setText("  " + videoName.mid(lastDot + 1).toUpper() + "  ");
    ui->labVideoName->setText(videoName);
}

void QVSLMainWindow::setPlayListWidgetVisable() {
    ui->playListWidget->setVisible(!this->isFullScreen());
}

void QVSLMainWindow::on_btnAdds_clicked()
{
    QString curPath = QDir::homePath();
    QString title = tr("请选择视频文件");
    QString filter =
        tr("MP4 AVI WMA RMVB MKV(*.mp4 *avi *.wma *.rmvb *.mkv);;所有文件(*.*)");

    QStringList vFNames = QFileDialog::getOpenFileNames(this,
                                                        title,
                                                        curPath,
                                                        filter);

    // no open files
    if (vFNames.count() < 1) {
        return;
    }

    for (int i = 0; i < vFNames.count(); ++i) {
        QString   vFName = vFNames.at(i);
        QFileInfo vFInfo(vFName);

        playlist->addMedia(QUrl::fromLocalFile(vFName));

        QListWidgetItem *item = new QListWidgetItem;
        item->setText(vFInfo.fileName());
        item->setToolTip(vFInfo.fileName());
        ui->listVideoWidget->addItem(item);
    }
    ui->listVideoWidget->setCurrentRow(0);

    if (player->state() != QMediaPlayer::PlayingState) {
        playlist->setCurrentIndex(0);
        player->play();
    }
}
