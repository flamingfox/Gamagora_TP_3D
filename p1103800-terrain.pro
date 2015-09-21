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
    interpolation.cpp \
    camera.cpp \
    rayon.cpp \
    box.cpp \
    terrain.cpp \
    terrain/terrainnoise.cpp \
    terrain/terraintab.cpp \
    terrain/terrain2.cpp

HEADERS +=  mesh.h \
    noisegenerator.h \
    zoneterrain.h \
    parametres.h \
    interpolation.h \
    camera.h \
    rayon.h \
    box.h \
    terrain.h \
    terrain/terrainnoise.h \
    terrain/terraintab.h \
    terrain/terrain2.h
