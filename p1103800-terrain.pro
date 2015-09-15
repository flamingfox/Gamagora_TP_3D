TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

INCLUDEPATH += C:/librairie
INCLUDEPATH += C:\lib
INCLUDEPATH += C:\Users\Rudi\Desktop\eigen3

SOURCES += main.cpp \
    mesh.cpp \
    noisegenerator.cpp \
    zoneterrain.cpp \
    terrain.cpp \
    interpolation.cpp \
    camera.cpp \
    rayon.cpp \
    box.cpp

HEADERS +=  mesh.h \
    noisegenerator.h \
    zoneterrain.h \
    terrain.h \
    parametres.h \
    interpolation.h \
    camera.h \
    rayon.h \
    box.h
