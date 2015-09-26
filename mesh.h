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
#include "float.h"

#include "noisegenerator.h"

#include "zoneterrain.h"
#include "terrain/terrain2.h"


class Mesh{

protected:
    std::vector<Eigen::Vector3f> geom;
    std::vector<Eigen::Vector3f> normals; //!!!! a ajouté !!!!
    std::vector<int> topo;

public :

    Mesh(){}
    Mesh(const std::vector<Eigen::Vector3f> listGeom, const std::vector<int> listTopo): geom(listGeom), topo(listTopo)
    {}

    Mesh(const Terrain2& terrain, int nbHeight, int nbWidth);

    ~Mesh(){}

    void Translation(const Eigen::Vector3f T);
    void Translation(const float x, const float y, const float z);
    void Rotation(const Eigen::Vector3f T);
    void Rotation(const float rX, const float rY, const float rZ);

    #define DEFAULT_RESOLUTION 32
    static Mesh cylindre(const Eigen::Vector3f& centreCercleA, const Eigen::Vector3f& centreCercleB, const float rayon, int resolution = DEFAULT_RESOLUTION);
    static Mesh cone(const Eigen::Vector3f &centreCercle, const Eigen::Vector3f &pointe, const float rayon, int resolution = DEFAULT_RESOLUTION);
    static Mesh sphere(const Eigen::Vector3f& centre, const float rayon, int resolution = DEFAULT_RESOLUTION);
    static Mesh galette(const Eigen::Vector3f& centre, const float rayon, int resolution = DEFAULT_RESOLUTION);

    static Mesh arbreGalette(const Eigen::Vector3f &centreCercleA, const Eigen::Vector3f &centreCercleB, const float rayon1, const Eigen::Vector3f &centreCercleC, const float rayon2);
    static Mesh arbreSpherique(const Eigen::Vector3f &centreCercleA, const Eigen::Vector3f &centreCercleB, const float rayon1, const Eigen::Vector3f &centreCercleC, const float rayon2);
    static Mesh arbreConique(const Eigen::Vector3f &centreCercleA, const Eigen::Vector3f &centreCercleB, const float rayon1, const Eigen::Vector3f &centreCercleC, const Eigen::Vector3f &pointe, const float rayon2);

    static Mesh generationArbre();

    //void load(string load);
    void save(const std::string name);
    void merge(const Mesh &delta);

    void rescale(float scale);

    std::vector<Eigen::Vector3f> getGeom() const;
    std::vector<int> getTopo() const;
    void setGeom(std::vector<Eigen::Vector3f> geom);
    void setTopo(std::vector<int> topo);
    size_t nbGeom() const;
    size_t nbTopo() const;


    void normalsTriangles();
    Eigen::Vector3f normalTriangle(int i) const;

    void simpleInitTopoTerrain(int nbHeight, int nbWidth);

    //std::pair<Eigen::Vector3f,Eigen::Vector3f> calculBoite();

private :

    static Mesh generationSphere(const Eigen::Vector3f& centre, const float rayon, const int resolution = DEFAULT_RESOLUTION);


protected:

    inline void addTopo(int i0, int i1, int i2);
};

inline void Mesh::addTopo(int i0, int i1, int i2)
{
    this->topo.push_back(i0);
    this->topo.push_back(i1);
    this->topo.push_back(i2);
}

#endif // MESH_H
