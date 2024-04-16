QT       += core gui
QT       += network
QT       += core5compat
QT       += xml
QT       += charts
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    connect/broadcastudp.cpp \
    connect/muliteudp.cpp \
    connect/tcp.cpp \
    connect/udp.cpp \
    dialog/addseriesdialog.cpp \
    dialog/createchartformdialog.cpp \
    dialog/querydialog.cpp \
    dialog/settingdialog.cpp \
    fmodchart/fchartform.cpp \
    fmodchart/fchartformmanager.cpp \
    fmodchart/icharform.cpp \
    main.cpp \
    mainwindow.cpp \
    other/createconnect.cpp \
    other/createserver.cpp \
    other/form.cpp \
    other/forward.cpp \
    other/length.cpp \
    other/setting.cpp \
    tab/tab.cpp \
    tab/tabserver.cpp \
    template/templateentity.cpp \
    template/templates.cpp \
    template/templatetables.cpp

HEADERS += \
    connect/broadcastudp.h \
    connect/muliteudp.h \
    connect/tcp.h \
    connect/udp.h \
    dialog/addseriesdialog.h \
    dialog/createchartformdialog.h \
    dialog/querydialog.h \
    dialog/settingdialog.h \
    fmodchart/fchartform.h \
    fmodchart/fchartformmanager.h \
    fmodchart/fmodchart.h \
    fmodchart/icharform.h \
    mainwindow.h \
    other/createconnect.h \
    other/createserver.h \
    other/forward.h \
    other/length.h \
    other/setting.h \
    other/form.h \
    tab/tab.h \
    tab/tabserver.h \
    template/templateentity.h \
    template/templates.h \
    template/templatetables.h

FORMS += \
    dialog/addseriesdialog.ui \
    dialog/createchartformdialog.ui \
    dialog/querydialog.ui \
    dialog/settingdialog.ui \
    fmodchart/fchartform.ui \
    mainwindow.ui \
    other/createconnect.ui \
    other/createserver.ui \
    other/form.ui \
    other/forward.ui \
    other/length.ui \
    other/setting.ui \
    tab/tab.ui \
    tab/tabserver.ui \
    template/templates.ui \
    template/templatetables.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    icon/allconnect.png \
    icon/allunconnect.png \
    icon/chuankoumodel.png \
    icon/clientmodel.png \
    icon/conencted.png \
    icon/connect.png \
    icon/connenctserver.png \
    icon/createchuankou.png \
    icon/createconnect.png \
    icon/createserver.png \
    icon/delete.png \
    icon/exit.png \
    icon/relate.png \
    icon/save.png \
    icon/savecon.png \
    icon/servermodel.png \
    icon/sloveall.png \
    icon/table.png \
    icon/title.png \
    icon/view.png

RESOURCES += \
    res.qrc
