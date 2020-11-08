Release: QMAKE_CXXFLAGS = /MT

QT += quick qml

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        zdwindow.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    logo.h \
    zdwindow.h




INCLUDEPATH += /usr/local/include/
INCLUDEPATH += /Users/zidizhang/Downloads/bgfx/bgfx/include/
INCLUDEPATH += /Users/zidizhang/Downloads/bgfx/bimg/include/
INCLUDEPATH += /Users/zidizhang/Downloads/bgfx/bx/include/

LIBS += -L/usr/local/lib/ -lSDL2
LIBS += -framework CoreGraphics -framework CoreFoundation -framework Foundation \
            -framework AppKit -framework IOKit -framework QuartzCore -framework Metal

LIBS += -L/Users/zidizhang/QtProjects/QTOpenGL/osx -lbgfxRelease -lbimg_decodeRelease  -lbimgRelease -lbxRelease



