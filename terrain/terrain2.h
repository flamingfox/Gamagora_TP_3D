#ifndef TERRAIN2_H
#define TERRAIN2_H

#include "./object.h"
#include "./rendu/colorgradient.h"
#include "./parametres.h"
#include "./noisegenerator.h"

class Terrain2: public Object
{
public:
    Terrain2();
    Terrain2(float longueur, float largeur);
    Terrain2(float longueur, float largeur, float amplitude);

    float longueur, largeur;

    float getVal(const Vector3f& p) const;


    /// \brief Terrain::getHauteur
    /// \param x abscisse entre 0 et largeur
    /// \param y ordonnee entre 0 et longueur
    /// \return return the elevation of the terrain in x,y
    float getHauteur(float x, float y) const;
    float getHauteur(const Vector2f& pointXY) const;
    float getHauteur(const Vector3f& pointXYZ) const;



    /// \brief Terrain::getNormal
    /// \param x abscisse entre 0 et largeur
    /// \param y ordonnee entre 0 et longueur
    /// \return return the normal of the terrain in x,y
    Eigen::Vector3f getNormal(float x, float y) const;
    Eigen::Vector3f getNormal(const Vector2f& pointXY) const;
    Eigen::Vector3f getNormal(const Vector3f& pointXYZ) const;

    bool inOut(const Eigen::Vector3f& pointXYZ) const;



    virtual float getHauteurXY(float x, float y) const = 0; //public car utilisé dans la classe Mesh

    //Eigen::Vector3f normale;
    ColorGradient heatMapGradient;    // Used to create a nice array of different colors.

protected:

    /// \brief Terrain::getNormal
    /// \param x abscisse entre 0 et 1
    /// \param y ordonnee entre 0 et 1
    /// \return return the normal of the terrain in x,y
    virtual Eigen::Vector3f getNormalXY(float x, float y) const = 0;

    /// \brief Terrain::intersectWithFunction
    /// \param rayon
    /// \param coeffDistance return the result of distance in this variable
    /// \param i return the number of iterations
    /// \return find the intersection distance of a ray (given in parameters) with the terrain
    bool intersect2(const Rayon& rayon, float &coeffDistance) const;
    void translate2(const Vector3f& t);
    void getColor2(float& r, float& g, float& b, float hauteur = 0, const Eigen::Vector3f& n = Eigen::Vector3f(0,0,1)) const;
    void getColor3(float& r, float& g, float& b, float x, float y) const;

    virtual float getMinElevation2() const = 0;
    virtual float getMaxElevation2() const = 0;

};

#endif // TERRAIN2_H
