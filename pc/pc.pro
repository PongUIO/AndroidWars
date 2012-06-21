######################################################################
# Automatically generated by qmake (2.01a) Thu Jun 2 17:40:16 2011
######################################################################

TEMPLATE = app
TARGET = ./build
DEPENDPATH += .
INCLUDEPATH += . ../simulation
LIBS += -L../build/ -lAWSim

# Input
HEADERS += main.h ../openglqt/render.h ../util/camera.h \
    ../util/cursordefines.h \
    ../util/client.h \
    main.h \
    ../openglqt/globj.h \
    ../openglqt/gamemap.h \
    botselector.h \
    gamebutton.h \
    gamecontroller.h \
    gameslider.h \
    menudefines.h \
    menubutton.h
SOURCES += main.cpp \
    botselector.cpp \
    gamebutton.cpp \
    gamecontroller.cpp \
    gameslider.cpp \
    menubutton.cpp
QT += opengl

OTHER_FILES += \
    ../shaders/animation.frag \
    ../shaders/animation.vert
