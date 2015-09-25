TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

INCLUDEPATH += C:/librairie
INCLUDEPATH += C:\lib
INCLUDEPATH += C:\Users\Rudi\Desktop\eigen3

QMAKE_CXXFLAGS += -fopenmp
LIBS += -fopenmp

SOURCES += main.cpp \
    mesh.cpp \
    noisegenerator.cpp \
    zoneterrain.cpp \
    interpolation.cpp \
    box.cpp \
    terrain.cpp \
    terrain/terrainnoise.cpp \
    terrain/terraintab.cpp \
    terrain/terrain2.cpp \
    rendu/scene.cpp \
    rendu/rayon.cpp \
    rendu/camera.cpp \
    lumiere/lumiere.cpp \
    object.cpp \
    rendu/colorgradient.cpp

HEADERS +=  mesh.h \
    noisegenerator.h \
    zoneterrain.h \
    parametres.h \
    interpolation.h \
    box.h \
    terrain.h \
    terrain/terrainnoise.h \
    terrain/terraintab.h \
    terrain/terrain2.h \
    rendu/scene.h \
    rendu/rayon.h \
    rendu/camera.h \
    lumiere/lumiere.h \
    object.h \
    rendu/colorgradient.h
