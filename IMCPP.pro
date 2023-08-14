QT       += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

RC_ICONS = app.ico

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addfrienddia.cpp \
    addgroupdia.cpp \
    admincon.cpp \
    admininfo.cpp \
    creategroupdia.cpp \
    dataprocessor.cpp \
    departcon.cpp \
    departmodel.cpp \
    friendcon.cpp \
    friendinfo.cpp \
    friendmodel.cpp \
    groupcon.cpp \
    groupmodel.cpp \
    logincon.cpp \
    loginmodel.cpp \
    loginui.cpp \
    main.cpp \
    mainwindow.cpp \
    qsocket.cpp \
    registerui.cpp \
    retrieveui.cpp

HEADERS += \
    addfrienddia.h \
    addgroupdia.h \
    admincon.h \
    admininfo.h \
    creategroupdia.h \
    dataprocessor.h \
    departcon.h \
    departmodel.h \
    friendcon.h \
    friendinfo.h \
    friendmodel.h \
    groupcon.h \
    groupmodel.h \
    logincon.h \
    loginmodel.h \
    loginui.h \
    mainwindow.h \
    public.h \
    qsocket.h \
    registerui.h \
    retrieveui.h

FORMS += \
    addfrienddia.ui \
    addgroupdia.ui \
    admininfo.ui \
    creategroupdia.ui \
    friendinfo.ui \
    loginui.ui \
    mainwindow.ui \
    registerui.ui \
    retrieveui.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    C:/Users/27836/Downloads/line-discuss（讨论）.png

RESOURCES += \
    res.qrc
