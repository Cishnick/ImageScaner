#-------------------------------------------------
#
# Project created by QtCreator 2020-03-20T19:30:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImageScaner
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        ImageProcessor/filterhandler.cpp \
        ImageProcessor/furietransformhandler.cpp \
        ImageProcessor/imageprocessor.cpp \
        OptionFilterWidget.cpp \
        Parametres/parametres.cpp \
        VectorScaner/paramvswidget.cpp \
        VectorScaner/plotter.cpp \
        VectorScaner/plotterwid.cpp \
        VectorScaner/vectoreditor.cpp \
        VectorScaner/vectorscan.cpp \
        imageprocessorshall.cpp \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        ImageProcessor/defines.h \
        ImageProcessor/filterhandler.h \
        ImageProcessor/furietransformhandler.h \
        ImageProcessor/ihandler.h \
        ImageProcessor/iimageprocessor.h \
        ImageProcessor/imageprocessor.h \
        OptionFilterWidget.h \
        Parametres/iparamdata.h \
        Parametres/iparametres.h \
        Parametres/iparamwidget.h \
        Parametres/parametres.h \
        VectorScaner/factoryparametresvs.h \
        VectorScaner/graphicclasses.h \
        VectorScaner/iplotter.h \
        VectorScaner/ivectoreditor.h \
        VectorScaner/ivectorscan.h \
        VectorScaner/paramdata.h \
        VectorScaner/paramdata_set.h \
        VectorScaner/paramvswidget.h \
        VectorScaner/plotter.h \
        VectorScaner/plotterwid.h \
        VectorScaner/vectoreditor.h \
        VectorScaner/vectorscan.h \
        VectorScaner/vectorwidget.h \
        imageprocessorshall.h \
        mainwindow.h \
        totaldefines.h

FORMS += \
        mainwindow.ui \
        optionfilterwidget.ui \
        paramvswidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    imagescanermodel.qmodel \
    parametres.qmodel
