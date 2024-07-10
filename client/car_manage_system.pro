QT       += core gui
QT       += network
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/qxlsx/include
LIBS += -L $$PWD/qxlsx/lib -l QXlsx

SOURCES += \
    carinfo.cpp \
    clientmanage.cpp \
    companyinfoview.cpp \
    fixrecord.cpp \
    loginsettings.cpp \
    loginwidget.cpp \
    main.cpp \
    homewidget.cpp \
    optlogview.cpp \
    sqliteclient.cpp \
    stuffmanage.cpp \
    syssettingview.cpp \
    systemwarning.cpp \
    usermanageview.cpp

HEADERS += \
    carinfo.h \
    clientmanage.h \
    companyinfoview.h \
    fixrecord.h \
    homewidget.h \
    loginsettings.h \
    loginwidget.h \
    optlogview.h \
    socket_data.h \
    sqliteclient.h \
    stuffmanage.h \
    syssettingview.h \
    systemwarning.h \
    usermanageview.h

FORMS += \
    carinfo.ui \
    clientmanage.ui \
    companyinfoview.ui \
    fixrecord.ui \
    homewidget.ui \
    loginsettings.ui \
    loginwidget.ui \
    optlogview.ui \
    stuffmanage.ui \
    syssettingview.ui \
    systemwarning.ui \
    usermanageview.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
