TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle

INCLUDEPATH += C:/librairie/Eigen/include

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

