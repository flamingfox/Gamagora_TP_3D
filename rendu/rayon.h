#ifndef RAYON_H
#define RAYON_H

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <math.h>


/**
 * @brief Classe représentant un rayon lumineux
 */
class Rayon
{
protected:
    /**
     * @brief position d'origine du rayon.
     */
    Eigen::Vector3f _origine;
    /**
     * @brief direction du rayon.
     */
    Eigen::Vector3f _direction;

public:

    Rayon();

    /**
     * @brief Constructeur paramètré.
     * @param[in] Position d'origine du rayon.
     * @param[in] Vecteur direction du rayon.
     */
    Rayon(const Eigen::Vector3f& origine, const Eigen::Vector3f& direction);

    /**
     * @brief Calcul la direction du rayon réfléchie par rapport à une surface.
     * @param[in] normal La normale à de la surface.
     * @return Le vecteur direction du rayon après réflection.
     */
    Eigen::Vector3f Reflect(const Eigen::Vector3f& normal);

    inline Eigen::Vector3f getOrigine()const{return _origine;}

    inline Eigen::Vector3f getDirection()const{return _direction;}
    inline void setDirection(const Eigen::Vector3f& dir){ _direction = dir.normalized();}

};

#endif // RAYON_H
