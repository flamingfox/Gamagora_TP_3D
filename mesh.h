#ifndef MESH_H
#define MESH_H

#include <Eigen/Core>
#include <Eigen/Geometry>

#include <string>
#include <vector>

#include <iostream>
#include <iomanip>
#include <fstream>

#include <math.h>

#include "noisegenerator.h"

#include "zoneterrain.h"

class Mesh{

protected:
    std::vector<Eigen::Vector3f> geom;
    //std::vector<Eigen::Vector3f> normal; //!!!! a ajouté !!!!
    std::vector<int> topo;

public :

    Mesh(const std::vector<Eigen::Vector3f> listGeom, const std::vector<int> listTopo): geom(listGeom), topo(listTopo)
    {};

    Mesh(){};

    void Translation(const Eigen::Vector3f T);
    void Translation(const float x, const float y, const float z);
    void Rotation(const Eigen::Vector3f T);
    void Rotation(const float rX, const float rY, const float rZ);


    static Mesh cylindre(const Eigen::Vector3f& centreCercleA, const Eigen::Vector3f& centreCercleB, const float rayon);
    static Mesh cylindre(const Eigen::Vector3f& centreCercleA, const Eigen::Vector3f& centreCercleB, const float rayon, const int resolution);
    static Mesh cone(const Eigen::Vector3f &centreCercle, const Eigen::Vector3f &pointe, const float rayon);
    static Mesh cone(const Eigen::Vector3f &centreCercle, const Eigen::Vector3f &pointe, const float rayon, const int resolution);
    static Mesh sphere(const Eigen::Vector3f& centre, const float rayon);
    static Mesh sphere(const Eigen::Vector3f& centre, const float rayon, const int resolution);
    static Mesh galette(const Eigen::Vector3f& centre, const float rayon);
    static Mesh galette(const Eigen::Vector3f& centre, const float rayon, const int resolution);

    static Mesh arbreGalette(const Eigen::Vector3f &centreCercleA, const Eigen::Vector3f &centreCercleB, const float rayon1, const Eigen::Vector3f &centreCercleC, const float rayon2);
    static Mesh arbreSpherique(const Eigen::Vector3f &centreCercleA, const Eigen::Vector3f &centreCercleB, const float rayon1, const Eigen::Vector3f &centreCercleC, const float rayon2);
    static Mesh arbreConique(const Eigen::Vector3f &centreCercleA, const Eigen::Vector3f &centreCercleB, const float rayon1, const Eigen::Vector3f &centreCercleC, const Eigen::Vector3f &pointe, const float rayon2);

    static Mesh generationArbre();

    //void load(string load);
    void save(const std::string name);
    void merge(const Mesh &delta);

    void rescale(float scale);

    std::vector<Eigen::Vector3f> getGeom();
    std::vector<int> getTopo();
    void setGeom(std::vector<Eigen::Vector3f> geom);
    void setTopo(std::vector<int> topo);

private :

    static Mesh generationSphere(const Eigen::Vector3f& centre, const float rayon);
    static Mesh generationSphere(const Eigen::Vector3f& centre, const float rayon, const int resolution);

};


#endif // MESH_H
