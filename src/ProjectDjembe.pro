#-------------------------------------------------
#
# Project created by QtCreator 2016-10-30T00:07:28
#
#-------------------------------------------------

QT       += core gui\
            serialport

QT       += multimedia
QT       += multimediawidgets

QTPLUGIN += qico


#CONFIG(release, debug|release): QMAKE_CXXFLAGS += -O2 -fp:fast -GL -Gw
#QMAKE_CXXFLAGS += -Wall



greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ProjectDjembe
TEMPLATE = app


LIBS += -L"D:\Desktop\Programme C++\ProjectDjembe\FFTW" -lfftw3-3 \
        -L"D:\Desktop\Programme C++\ProjectDjembe\FFTW" -lfftw3f-3

INCLUDEPATH += $$PWD/FFFT/include\
               $$PWD/SFML/include \

CONFIG(release, debug|release): LIBS += -L"D:\Desktop\Programme C++\ProjectDjembe\SFML\SFML-Qt_Build-Release\lib" \
                                        -lsfml-audio -lsfml-graphics -lsfml-main -lsfml-network -lsfml-window -lsfml-system
CONFIG(debug, debug|release): LIBS += -L"D:\Desktop\Programme C++\ProjectDjembe\SFML\SFML-Qt_Build-Debug\lib" \
                                      -lsfml-audio-d -lsfml-graphics-d -lsfml-main-d -lsfml-network-d -lsfml-window-d -lsfml-system-d

SOURCES += main.cpp\
        projectdjembe.cpp\
        recordandcorrect.cpp \
    audioinfo.cpp \
    graphics.cpp \
    engine.cpp \
    arduinomanager.cpp \
    settings.cpp \
    hra.cpp


HEADERS  += projectdjembe.h\
            recordandcorrect.h\
    audioinfo.h \
    graphics.h \
    fftw3.h \
    utils.h \
    engine.h \
    arduinomanager.h \
    settings.h \
    hra.h \
    datatypes.h


# Ensure that library is built before application
CONFIG  += ordered

FORMS    += projectdjembe.ui\
            recordandcorrect.ui \
            settings.ui

RESOURCES += \
    resources.qrc


#RC_FILE = Icons/appicon.rc
