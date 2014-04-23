#-------------------------------------------------
#
# Project created by QtCreator 2012-02-08T11:00:55
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = branchometer
TEMPLATE = app

SOURCES += main.cpp\
           mainwindow.cpp \
           glwidget.cpp \
           basictool.cpp
# Mac
HEADERS  += mainwindow.h \
    glwidget.h \
    basctype.h \
    fravar.h \
    fratype.h \
    basictool.h

# Windows
#HEADERS  += mainwindow.h \
#    glwidget.h \
#    basctype.h \
#    fravar.h \
#    fratype.h \
#    basictool.h\
#    glut.h

FORMS    += mainwindow.ui

RESOURCES += \
    dockwidgets.qrc \
    do.qrc

#Mac
LIBS += -framework GLUT

# Windows
#LIBS += -lgut32 \
#        -LC:\glut

# Linux / X11
#LIBS += -lGL -lGLU -lglut
