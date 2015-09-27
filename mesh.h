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
#include "QImage"

#include "noisegenerator.h"

#include "interpolation.h"
#include "terrain/terrain.h"

using namespace Eigen;
class Mesh{

protected:
    std::vector<Eigen::Vector3f> geom;
    std::vector<Eigen::Vector3f> normalsPoints;
    std::vector<int> topo;
    int nbPointLongueur, nbPointLargeur;
    int longueur, largeur;

public :

    Mesh(){}

    Mesh(const std::vector<Eigen::Vector3f> listGeom, const std::vector<int> listTopo): geom(listGeom), topo(listTopo)  {}

    Mesh(const QImage &img, float _longueur, float _largeur, float amplitude);
    Mesh(const QImage& img, float _longueur, float _largeur, float _amplitude, int _nbHeight, int _nbWidth);

    Mesh(const Terrain& terrain, int nbHeight, int nbWidth);

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

    //trieeee=============================================

    void applicationSin(int amplitude, int periode);
    void applicationWarp(int amplitude, int periode);
    void applicationRidge(float seuil, float amplitude, int periode);
    void applicationNoise(int amplitude, int periode);

    inline const Vector3f& getPoint(int i, int j) const;
    inline const Vector3f& getPoint(const Eigen::Vector2i& pos) const;
    inline const Vector3f& getN(int i, int j) const;
    inline const Vector3f& getN(const Eigen::Vector2i& pos) const;



    Vector3f getNormal(float pointX, float pointY) const;

    inline Vector3f getNormal(const Vector2f &pointXY) const;

    void simpleInitTopoTerrain(int nbHeight, int nbWidth);

private :

    static Mesh generationSphere(const Eigen::Vector3f& centre, const float rayon, const int resolution = DEFAULT_RESOLUTION);


protected:

    inline void addTopo(int i0, int i1, int i2);
};




/*************************************inline********************************/



inline void Mesh::addTopo(int i0, int i1, int i2)
{
    this->topo.push_back(i0);
    this->topo.push_back(i1);
    this->topo.push_back(i2);
}





#endif // MESH_H
