#include "qvslmainwindow.h"
#include "ui_qvslmainwindow.h"

void QVSLMainWindow::mousePressEvent(QMouseEvent *event)
{
    this->mousePress(event);
    QWidget::mousePressEvent(event);
}

void QVSLMainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    this->mouseRelease(event, false);
    QWidget::mouseReleaseEvent(event);
}

void QVSLMainWindow::mouseMoveEvent(QMouseEvent *event)
{
    this->mouseMove(event);
    QWidget::mouseMoveEvent(event);
}

void QVSLMainWindow::keyPressEvent(QKeyEvent *event)
{
    this->keyPress(event);
    QWidget::keyPressEvent(event);
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

    connect(player, SIGNAL(durationChanged(qint64)),
            this, SLOT(on_durationChanged(qint64)));
    connect(player, SIGNAL(positionChanged(qint64)),
            this, SLOT(on_positionChanged(qint64)));

    //    connect(player, SLOT(error(QMediaPlayer::Error)),
    //            this, SLOT(on_error(QMediaPlayer::Error)));

    // Niubility!!! //TO-DO Multiple triggered ???
    //    connect(
    //        player, static_cast<void
    // (QMediaPlayer::*)(QMediaPlayer::Error)>(&QMediaPlayer::error),
    //        this, &QVSLMainWindow::on_error);
    connect(playlist, SIGNAL(currentIndexChanged(int)),
            this, SLOT(on_currentIndexChanged(int)));

    initButtonState();

    ui->widgetVideo->setFocus();
}

QVSLMainWindow::~QVSLMainWindow()
{
    delete ui;
}

bool QVSLMainWindow::getPlaylistShownState()
{
    return this->isPlaylistShownState;
}

void QVSLMainWindow::on_btnList_clicked()
{
    ui->playListWidget->setVisible(!isPlaylistShownState);
    isPlaylistShownState = !isPlaylistShownState;
}

void QVSLMainWindow::on_btnMax_clicked()
{
    bool isMax = this->isMaximized();

    if (isMax) {
        isMaxState = false;
        ui->btnMax->setIcon(QIcon(":/icons/light/icons/light/icon_maxscreen.png"));
        ui->btnMax->setToolTip(tr("最大化"));
        this->showNormal();
    } else {
        isMaxState = true;
        ui->btnMax->setIcon(QIcon(
                                ":/icons/light/icons/light/icon_resetscreen.png"));
        ui->btnMax->setToolTip(tr("恢复尺寸"));
        this->showMaximized();
    }
}

void QVSLMainWindow::on_btnFullScreen_clicked()
{
    videoFullScreen();
}

void QVSLMainWindow::videoFullScreen() {
    if (this->isFullScreen()) {
        if (isMaxState) {
            this->showMaximized();
        }
        else {
            this->showNormal();
        }
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

void QVSLMainWindow::mouseRelease(QMouseEvent *event, bool clickFlag)
{
    if ((event->button() == Qt::LeftButton) && clickFlag) {
        this->videoPlayAndPause();
    }

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

void QVSLMainWindow::keyPress(QKeyEvent *event)
{
    if (isFullScreen() && (event->key() == Qt::Key_Escape)) {
        if (isMaxState) {
            this->showMaximized();
        }
        else {
            this->showNormal();
        }
        this->setTopAndBottomWidgetVisiable(true);
    }

    if (event->key() == Qt::Key_Space) {
        this->videoPlayAndPause();
    }

    if ((event->key() == Qt::Key_Enter) || (event->key() == Qt::Key_Return)) {
        this->videoFullScreen();
    }
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
    // no video file
    if (position < 0) {
        return;
    }

    ui->listVideoWidget->setCurrentRow(position);

    QListWidgetItem *curItem = ui->listVideoWidget->item(position);
    QString videoName = curItem->toolTip();
    int     lastDot = videoName.lastIndexOf('.');

    // set video name and type
    ui->labVideoType->setText("  " + videoName.mid(lastDot + 1).toUpper() + "  ");
    ui->labVideoName->setText(videoName);
}

void QVSLMainWindow::on_error(QMediaPlayer::Error error)
{
    player->stop();
    QString title = tr("播放失败");
    QString errMsg = tr("未知错误");

    switch (error) {
    case QMediaPlayer::NoError:
        errMsg = tr("无错误");
        break;

    case QMediaPlayer::ResourceError:
        errMsg = tr("资源加载错误");
        break;

    case QMediaPlayer::FormatError:
        errMsg = tr("媒体格式错误");
        break;

    case QMediaPlayer::NetworkError:
        errMsg = tr("网络连接错误");
        break;

    case QMediaPlayer::AccessDeniedError:
        errMsg = tr("无访问权限");
        break;

    case QMediaPlayer::ServiceMissingError:
        errMsg = tr("丢失服务");
        break;

    case QMediaPlayer::MediaIsPlaylist:
        errMsg = tr("媒体是播放列表");
        break;

    default:
        break;
    }

    on_btnStop_clicked();
    QMessageBox::critical(this, title, tr("媒体文件播放失败：%1").arg(errMsg));
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
        item->setForeground(QColor("white"));
        item->setText(vFInfo.fileName());
        item->setToolTip(vFInfo.fileName());
        item->setData(Qt::UserRole + 1, vFInfo.absoluteFilePath());
        ui->listVideoWidget->addItem(item);
    }
    ui->listVideoWidget->setCurrentRow(0);

    if (player->state() != QMediaPlayer::PlayingState) {
        playlist->setCurrentIndex(0);
        player->play();
        setPlayButtonIconPause();
    }
}

// delete items from playlist
void QVSLMainWindow::on_btnDelete_clicked()
{
    int plCurIndex = playlist->currentIndex();
    QModelIndexList selIndexes =
        ui->listVideoWidget->selectionModel()->selectedIndexes();

    if (selIndexes.count() == 0) {
        return;
    }

    QList<int> *selRows = new QList<int>();

    for (int i = 0; i < selIndexes.count(); ++i) {
        selRows->append(selIndexes.at(i).row());
    }

    // sort and reversed
    qSort(selRows->begin(), selRows->end(), moreThan);

    for (int i = 0; i < selRows->count(); ++i) {
        const int selRow = selRows->at(i);

        // stop current video
        if (selRow == plCurIndex) {
            player->stop();
            setPlayButtonIconPlay();
        }

        // remove media from playlist and delete item from QListWidget
        playlist->removeMedia(selRow);
        QListWidgetItem *selItem = ui->listVideoWidget->takeItem(selRow);
        delete selItem;
    }

    // no video files
    if (isVideoListEmpty()) {
        player->stop();
        setPlayButtonIconPlay();
        setVideoInfoEmpty();
    } else {
        if (player->state() == QMediaPlayer::StoppedState) {
            playlist->setCurrentIndex(0);
            on_currentIndexChanged(0);
            player->play();
            setPlayButtonIconPause();
        }
    }
}

bool QVSLMainWindow::moreThan(int a, int b)
{
    return a > b;
}

void QVSLMainWindow::setVideoInfoEmpty()
{
    ui->labVideoName->setText("");
    ui->labVideoType->setText("");
    ui->labDuration->setText("00:00:00/00:00:00");
}

void QVSLMainWindow::initButtonState()
{
    ui->btnInc->setVisible(false);
    ui->btnDesc->setVisible(false);
}

void QVSLMainWindow::on_listVideoWidget_doubleClicked(const QModelIndex& index)
{
    if (ui->listVideoWidget->currentIndex().row() < 0) {
        return;
    }
    player->stop();
    setPlayButtonIconPause();
    playlist->setCurrentIndex(index.row());
    player->play();
}

void QVSLMainWindow::on_btnPlay_clicked()
{
    videoPlayAndPause();
}

void QVSLMainWindow::videoPlayAndPause() {
    // empty play list
    if (isVideoListEmpty()) {
        return;
    }

    if (player->state() == QMediaPlayer::PlayingState) {
        player->pause();
        setPlayButtonIconPlay();
    } else {
        if (playlist->currentIndex() < 0) {
            playlist->setCurrentIndex(0);
        }
        player->play();
        setPlayButtonIconPause();
    }
}

void QVSLMainWindow::setPlayButtonIconPlay() {
    setPlayButtonIcon(QIcon(":/icons/light/icons/light/icon_play.png"));
}

void QVSLMainWindow::setPlayButtonIconPause() {
    setPlayButtonIcon(QIcon(":/icons/light/icons/light/icon_pause.png"));
}

void QVSLMainWindow::setPlayButtonIcon(QIcon icon) {
    ui->btnPlay->setIcon(icon);
}

bool QVSLMainWindow::isVideoListEmpty() {
    if (ui->listVideoWidget->count() < 1) {
        return true;
    }
    return false;
}

void QVSLMainWindow::on_btnStop_clicked()
{
    // empty play list
    if (isVideoListEmpty()) {
        return;
    }

    player->stop();
    setPlayButtonIconPlay();
    setVideoInfoEmpty();
}

void QVSLMainWindow::on_btnClear_clicked()
{
    player->stop();
    setPlayButtonIconPlay();
    playlist->clear();
    ui->listVideoWidget->clear();
    setVideoInfoEmpty();
}

void QVSLMainWindow::on_btnSkipBack_clicked()
{
    // empty play list
    if (isVideoListEmpty()) {
        return;
    }

    playlist->previous();
    player->play();
    setPlayButtonIconPause();
}

void QVSLMainWindow::on_btnSkipForward_clicked()
{
    // empty play list
    if (isVideoListEmpty()) {
        return;
    }

    playlist->next();
    player->play();
    setPlayButtonIconPause();
}

void QVSLMainWindow::on_btnRewind10_clicked()
{
    // empty play list
    if (isVideoListEmpty()) {
        return;
    }

    qint64 pos = qMax(player->position() - 10000, (qint64)0);

    player->setPosition(pos);
}

void QVSLMainWindow::on_btnForward10_clicked()
{
    // empty play list
    if (isVideoListEmpty()) {
        return;
    }
    qint64 pos = qMin(player->position() + 10000, player->duration());

    player->setPosition(pos);
}

void QVSLMainWindow::on_sliderVolumn_valueChanged(int value)
{
    player->setVolume(value);

    if (value == 0) {
        ui->btnVolumn->setIcon(QIcon(":/icons/light/icons/light/icon_mute.png"));
    } else {
        ui->btnVolumn->setIcon(QIcon(":/icons/light/icons/light/icon_volume.png"));
    }
}

void QVSLMainWindow::on_sliderProgress_valueChanged(int value)
{
    player->setPosition(value);
}

void QVSLMainWindow::on_btnVolumn_clicked()
{
    bool isMuted = player->isMuted();

    player->setMuted(!isMuted);

    if (isMuted) {
        ui->btnVolumn->setIcon(QIcon(":/icons/light/icons/light/icon_mute.png"));
    } else {
        ui->btnVolumn->setIcon(QIcon(":/icons/light/icons/light/icon_volume.png"));
    }
}

void QVSLMainWindow::on_btnOpen_clicked()
{
    QString curPath = QDir::homePath();
    QString title = tr("请选择视频文件");
    QString filter =
        tr("MP4 AVI WMA RMVB MKV(*.mp4 *avi *.wma *.rmvb *.mkv);;所有文件(*.*)");

    QString vFName = QFileDialog::getOpenFileName(this,
                                                  title,
                                                  curPath,
                                                  filter);

    // no open files
    if (vFName.isEmpty()) {
        return;
    }

    QFileInfo vFInfo(vFName);

    playlist->addMedia(QUrl::fromLocalFile(vFName));

    // add item to list
    QListWidgetItem *item = new QListWidgetItem;

    item->setForeground(QColor("white"));
    item->setText(vFInfo.fileName());
    item->setToolTip(vFInfo.fileName());
    ui->listVideoWidget->addItem(item);
    ui->listVideoWidget->setCurrentRow(ui->listVideoWidget->count() - 1);

    // play directly
    playlist->setCurrentIndex(ui->listVideoWidget->count() - 1);
    player->play();
    setPlayButtonIconPause();
}

void QVSLMainWindow::on_btnRefresh_clicked()
{
    QList<int> *reList = new QList<int>();
    int listCount = ui->listVideoWidget->count();

    // find invalid files
    for (int i = 0; i < listCount; ++i) {
        QListWidgetItem *aItem = ui->listVideoWidget->item(i);
        QString   filePath = aItem->data(Qt::UserRole + 1).toString();
        QFileInfo fileInfo(filePath);

        if (!fileInfo.exists()) {
            reList->append(i);
        }
    }

    if (!reList->isEmpty()) {
        qSort(reList->begin(), reList->end(), moreThan);

        // remove invalid files
        for (int i = 0; i < reList->count(); ++i) {
            int row = reList->at(i);
            QListWidgetItem *rmItem =
                ui->listVideoWidget->takeItem(row);
            delete rmItem;
            playlist->removeMedia(row);
        }
    }
}
