#-------------------------------------------------
#
# Project created by QtCreator 2016-06-05T12:25:27
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = UTComputer
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
        Atome.cpp \
        Complexe.cpp \
        Controleur.cpp \
        Entiere.cpp \
        Expression.cpp \
        Litterale.cpp \
        Numerique.cpp \
        Pile.cpp \
        Programme.cpp \
        projet.cpp \
        Rationelle.cpp \
        Reelle.cpp \
        ComputerException.cpp \
    LitteraleManager.cpp \
    param.cpp \
    Strategy.cpp

HEADERS  += mainwindow.h \
    Atome.hpp \
    Complexe.h \
    Controleur.hpp \
    Entiere.h \
    Expression.hpp \
    Litterale.hpp \
    Numerique.h \
    Pile.hpp \
    Programme.hpp \
    projet.h \
    Rationelle.h \
    Reelle.h \
    ComputerException.h \
    LitteraleManager.h \
    param.h \
    Strategy.hpp

FORMS    += mainwindow.ui \
    param.ui
