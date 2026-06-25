QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += sdk_no_version_check
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

QMAKE_APPLE_DEVICE_ARCHS = x86_64 arm64
QMAKE_MACOSX_DEPLOYMENT_TARGET = 13.0
ICON = i2m.icns

SOURCES += \
    aboutdialog.cpp \
    cimagedropview.cpp \
    cmatrix.cpp \
    cmosaic.cpp \
    cmosaicparams.cpp \
    cpalette.cpp \
    csearchresult.cpp \
    main.cpp \
    mainwindow.cpp \
    preferencesdialog.cpp \
    qsliderplus.cpp

HEADERS += \
    aboutdialog.h \
    cimagedropview.h \
    cmatrix.h \
    cmosaic.h \
    cmosaicparams.h \
    cpalette.h \
    csearchresult.h \
    mainwindow.h \
    preferencesdialog.h \
    qsliderplus.h

FORMS += \
    aboutdialog.ui \
    mainwindow.ui \
    preferencesdialog.ui \
    qsliderplus.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

#DISTFILES += \
#    ../../../Documents/MATLAB/Image/Lego Store 1x1 B&W.csv

# Link to custom Universal Exiv2 (see Step 2 for creating this path)
INCLUDEPATH += /usr/local/universal/include
LIBS += -L/usr/local/universal/lib \
    -lexiv2 \
    -linireader \
    -linih \
    -lexpat \
    -lz \
    -liconv \
    -lc++
DEPENDPATH += /usr/local/universal/include

DISTFILES += \
    README.md
