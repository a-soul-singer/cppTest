QT       += core gui
QT       += network
QT       += sql
QT       += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


# 指定第三方库的头文件目录
INCLUDEPATH += $$PWD/qxlsx/include
# 指定第三方库的库文件目录和具体链接的库文件
LIBS += -L $$PWD/qxlsx/lib -l QXlsx

SOURCES += \
    AES/qaesencryption.cpp \
    aesencryptionmanager.cpp \
    carinfo.cpp \
    clientmanage.cpp \
    companyinfoview.cpp \
    electronicinterface.cpp \
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
    AES/aesni-enc-cbc.h \
    AES/aesni-enc-ecb.h \
    AES/aesni-key-exp.h \
    AES/aesni-key-init.h \
    AES/qaesencryption.h \
    aesencryptionmanager.h \
    carinfo.h \
    clientmanage.h \
    companyinfoview.h \
    electronicinterface.h \
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
    electronicinterface.ui \
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
