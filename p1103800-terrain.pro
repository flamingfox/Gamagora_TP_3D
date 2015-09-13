TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += C:/lib
INCLUDEPATH += C:\Users\Rudi\Desktop\eigen3

SOURCES += main.cpp \
    mesh.cpp \
    noisegenerator.cpp \
    zoneterrain.cpp \
    generationvegetation.cpp \
    arbre.cpp \
    terrain.cpp

HEADERS += \
    Vecteur.hpp \
    mesh.h \
    noisegenerator.h \
    zoneterrain.h \
    generationvegetation.h \
    arbre.h \
    terrain.h
    terrain.h \
    parametres.h

