######################################################################
# Automatically generated by qmake (2.01a) Thu Jun 2 17:40:16 2011
######################################################################

TEMPLATE = app
TARGET = ./build
DEPENDPATH += .
INCLUDEPATH += . ../simulation
LIBS += -L../build/ -lAWSim

# Input
HEADERS += main.h ../openglqt/render.h customlabel.h ../util/camera.h \
    menubutton.h \
    gamecontroller.h
SOURCES += main.cpp
QT += opengl

RESOURCES += \
    res.qrc

OTHER_FILES += \
    graphics/tiles/metal.png \
    graphics/temp/temp.png \
    graphics/temp/temp2.png \
    graphics/tiles/metal2surf.png \
    graphics/characters/temp.png \
    graphics/menu/startgame.png
