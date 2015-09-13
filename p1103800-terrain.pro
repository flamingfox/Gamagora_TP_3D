TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle

INCLUDEPATH += C:/librairie/Eigen/include
INCLUDEPATH += C:/lib
INCLUDEPATH += C:\Users\Rudi\Desktop\eigen3

SOURCES += main.cpp \
    mesh.cpp \
    noisegenerator.cpp \
    zoneterrain.cpp \
    terrain.cpp

HEADERS +=  mesh.h \
    noisegenerator.h \
    zoneterrain.h \
    terrain.h \
    parametres.h
