QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TARGET = VideoSharing-LAN

SOURCES += \
    main.cpp \
    qudfloatwidget.cpp \
    qudvideowidget.cpp \
    qvslmainwindow.cpp

HEADERS += \
    qudfloatwidget.h \
    qudvideowidget.h \
    qvslmainwindow.h

FORMS += \
    qvslmainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    .github/workflows/android.yml \
    .github/workflows/ios.yml \
    .github/workflows/macos.yml \
    .github/workflows/ubuntu.yml \
    .github/workflows/windows-mingw.yml \
    .github/workflows/windows.yml \
    .gitignore \
    LICENSE \
    README.md \
    README_zh.md \
    scripts/windows-mingw-publish.ps1 \
    scripts/windows-publish.ps1

RC_ICONS = icons/icon_videosharing.ico

RESOURCES += \
    icons.qrc

DESTDIR = $$absolute_path($${_PRO_FILE_PWD_}/bin/release)
