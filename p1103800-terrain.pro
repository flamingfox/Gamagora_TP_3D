TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11
QMAKE_CXXFLAGS += -fopenmp
LIBS += -fopenmp

INCLUDEPATH += C:/librairie
INCLUDEPATH += C:\lib
INCLUDEPATH += C:\Users\Rudi\Desktop\eigen3

QMAKE_CXXFLAGS += -fopenmp
LIBS += -fopenmp

SOURCES += main.cpp \
    mesh.cpp \
    noisegenerator.cpp \
    interpolation.cpp \
    box.cpp \
    terrain/terrainnoise.cpp \
    terrain/terraintab.cpp \
    rendu/scene.cpp \
    rendu/rayon.cpp \
    rendu/camera.cpp \
    rendu/colorgradient.cpp \
    nrw.cpp \
    terrain/terrain.cpp

HEADERS +=  mesh.h \
    noisegenerator.h \
    parametres.h \
    interpolation.h \
    box.h \
    terrain/terrainnoise.h \
    terrain/terraintab.h \
    rendu/scene.h \
    rendu/rayon.h \
    rendu/camera.h \
    rendu/colorgradient.h \
    nrw.h \
    terrain/terrain.h
