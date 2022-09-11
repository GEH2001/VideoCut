QT       += core gui
QT       += multimedia
QT       += multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += $$PWD/ffmpeg/include


LIBS += -L$$PWD/"ffmpeg/lib" -lavcodec \
        -L$$PWD/"ffmpeg/lib" -lavdevice \
        -L$$PWD/"ffmpeg/lib" -lavfilter \
        -L$$PWD/"ffmpeg/lib" -lavformat \
        -L$$PWD/"ffmpeg/lib" -lavutil \
        -L$$PWD/"ffmpeg/lib" -lpostproc \
        -L$$PWD/"ffmpeg/lib" -lswresample \
        -L$$PWD/"ffmpeg/lib" -lswscale \

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    doubledialog.cpp \
    ffmpeg.cpp \
    fileop.cpp \
    getduration.cpp \
    infodialog.cpp \
    main.cpp \
    mainwindow.cpp \
    outdialog.cpp \
    tupiandialog.cpp

HEADERS += \
    doubledialog.h \
    ffmpeg.h \
    fileop.h \
    getduration.h \
    info.h \
    infodialog.h \
    mainwindow.h \
    outdialog.h \
    tupiandialog.h

FORMS += \
    doubledialog.ui \
    infodialog.ui \
    mainwindow.ui \
    outdialog.ui \
    tupiandialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc
