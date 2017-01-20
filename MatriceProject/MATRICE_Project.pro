#-------------------------------------------------
#
# Project created by QtCreator 2016-06-17T13:54:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MATRICE_Project
TEMPLATE = app

SOURCES += main.cpp \
    MainWindow.cpp \
    GraphicsScene.cpp \
    OneDollarRecognizerObserver.cpp \
    CurveItem.cpp \
    ColorPaletteWidget.cpp \
    Tree/TreeItem.cpp \
    Tree/TreeModel.cpp \
    Tree/TreeCurveWidget.cpp

HEADERS += \
    Leap.h \
    LeapMath.h \
    LgData.h \
    LgDataTemplate.h \
    LgEnvironment.h \
    LgFunctions.h \
    LgGenerator.h \
    LgGroupTemplate.h \
    LgHistoricTemplate.h \
    LgMetaData.h \
    LgNode.h \
    LgNodeFactory.h \
    LgObserver.h \
    LgPoint2D.h \
    LgPoint3D.h \
    LgProcessor.h \
    LgSwitch.h \
    LgTimer.h \
    MainWindow.h \
    GraphicsScene.h \
    OneDollarRecognizerObserver.h \
    CurveItem.h \
    ColorPaletteWidget.h \
    Tree/TreeItem.h \
    Tree/TreeModel.h \
    Tree/TreeCurveWidget.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../lib/ -lGina
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../lib/ -llibGina0.2d
else:unix: LIBS += -L$$PWD/../lib/ -lGina

INCLUDEPATH += $$PWD/../include
DEPENDPATH += $$PWD/../include

RESOURCES += \
    Resources.qrc
