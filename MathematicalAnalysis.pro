QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    neuronet.cpp \
    neuronet_m.cpp

HEADERS += \
    mainwindow.h \
    neuronet.h \
    neuronet_m.h

FORMS += \
    mainwindow.ui

INCLUDEPATH += \
    /usr/local/boost_1_79_0/ \
    /usr/local/include/

LIBS += "-L/usr/local/boost_1_79_0/stage/lib/"

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
