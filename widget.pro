#-------------------------------------------------
#
# Project created by QtCreator 2019-08-08T21:44:32
#
#-------------------------------------------------

QT       += core gui
QT      +=websockets serialport
QT       += webengine webenginewidgets multimedia multimediawidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = widget
TEMPLATE = app
CONFIG += resources_big

LIBS += -lzmq
# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_MESSAGELOGCONTEXT
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    bridge.cpp \
    card_dialog.cpp \
    cloud_deck.cpp \
    cloud_whistle.cpp \
    compass.cpp \
    compass_dialog.cpp \
    compass_widget.cpp \
    datareceive.cpp \
    define_widget.cpp \
    dialog_subdevlist.cpp \
    gps2bd.cpp \
    http_client.cpp \
        main.cpp \
    my_dialog.cpp \
    mythreadfromqthread.cpp \
    net_dialog.cpp \
    netcan_dev.cpp \
    serialport.cpp \
    serialportlist.cpp \
    slecte_dialog.cpp \
    tcp_client.cpp \
    tcp_socket.cpp \
    thread.cpp \
    wavedetector_cd.cpp \
    wavedetector_dev.cpp \
    websocket_qjon.cpp \
        widget.cpp \
    net_widget.cpp \
    detect_widget.cpp \
    mainwindow.cpp \
    slider_button.cpp \
    defense_widget.cpp \
    location_widget.cpp \
    dashboard.cpp \
    zeromqcli.cpp

HEADERS += \
    bridge.h \
    card_dialog.h \
    cloud_deck.h \
    cloud_whistle.h \
    cmaskgenerator.h \
    compass.h \
    compass_dialog.h \
    compass_widget.h \
    datareceive.h \
    define_widget.h \
    dialog_subdevlist.h \
    gps2bd.h \
    http_client.h \
    my_dialog.h \
    mythreadfromqthread.h \
    net_dialog.h \
    netcan_dev.h \
    serialport.h \
    serialportlist.h \
    slecte_dialog.h \
    strtohex.h \
    tcp_client.h \
    tcp_socket.h \
    thread.h \
    wavedetector_cd.h \
    wavedetector_dev.h \
    websocket_qjon.h \
        widget.h \
    net_widget.h \
    detect_widget.h \
    mainwindow.h \
    slider_button.h \
    defense_widget.h \
    location_widget.h \
    dashboard.h \
    zeromqcli.h

RESOURCES += \
    icon.qrc

FORMS += \
    card_dialog.ui \
    mainwindow.ui



