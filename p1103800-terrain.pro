TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

INCLUDEPATH += C:/librairie
INCLUDEPATH += C:\lib
INCLUDEPATH += C:\Users\Rudi\Desktop\eigen3

SOURCES += main.cpp \
    lumiere/lumiere.cpp \
    mesh/terrain/terrain.cpp \
    mesh/terrain/zoneterrain.cpp \
    mesh/box.cpp \
    mesh/mesh.cpp \
    outils/interpolation.cpp \
    scene/camera.cpp \
    scene/rayon.cpp \
    scene/scene.cpp \
    mesh/forme/cube.cpp \
    mesh/terrain/noisegenerator.cpp

HEADERS +=  lumiere/lumiere.h \
    lumiere/phong.h \
    mesh/terrain/terrain.h \
    mesh/terrain/zoneterrain.h \
    mesh/box.h \
    mesh/mesh.h \
    outils/interpolation.h \
    outils/parametres.h \
    scene/camera.h \
    scene/rayon.h \
    scene/scene.h \
    mesh/forme/cube.h \
    mesh/terrain/noisegenerator.h

