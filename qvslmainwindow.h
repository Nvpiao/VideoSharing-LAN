#ifndef QVSLMAINWINDOW_H
#define QVSLMAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class QVSLMainWindow; }
QT_END_NAMESPACE

class QVSLMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QVSLMainWindow(QWidget *parent = nullptr);
    ~QVSLMainWindow();

private:
    Ui::QVSLMainWindow *ui;
};
#endif // QVSLMAINWINDOW_H
