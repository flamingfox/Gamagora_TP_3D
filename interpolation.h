#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include <math.h>
#include <Eigen/Geometry>


using namespace Eigen;

/**
* @namespace interp
* espace de nommage des fonction d'interpolation
* (le code a été écrit sur la base que Y est la hauteur et Z est l'ordonnée (pour blender par exemple))
*/
namespace interp
{

/**
* @brief calcul de la pente
* @param[in] val1 valeur à la position 1
* @param[in] val2 valeur à la position 2
* @param[in] p1 position 1
* @param[in] p2 position 2
* @return augmentation de la valeur sur une distance d'unité 1
*/
float pente(float val1, float val2, float p1 = 0, float p2 = 1);

/**
* @brief calcul d'une valeur sur un segment entre 2 positions
* @param[in] abis position entre a1 et a2
* @param[in] val1 valeur à la position 1
* @param[in] val2 valeur à la position 2
* @param[in] a1 position 1
* @param[in] a2 position 2
* @return la valeur à la position abis
*/
float interp_linear1D(float abis, float val1, float val2, float a1 = 0, float a2 = 1);

/**
* @brief calcul d'une valeur sur une courbe en cos entre 2 positions
* @param[in] abis position entre a1 et a2
* @param[in] val1 valeur à la position 1
* @param[in] val2 valeur à la position 2
* @param[in] a1 position 1
* @param[in] a2 position 2
* @return la valeur à la position abis
*/
float interp_cos1D(float abis, float val1, float val2, float a1 = 0, float a2 = 1);

/**
* @brief calcul d'une valeur sur une courbe d'hermite entre 2 positions
* @param[in] abis position entre a1 et a2
* @param[in] val1 valeur à la position 1
* @param[in] val2 valeur à la position 2
* @param[in] a1 position 1
* @param[in] a2 position 2
* @return la valeur à la position abis
*/
float interp_hermite1D(float abis, float val1, float val2, float a1 = 0, float a2 = 1);

/*************************************************************************/

/**
* @brief calcul d'une valeur sur une surface plane/courbé entre 4 positions (segments sur les bords)
* @param[in] xb x entre x1 et x2
* @param[in] zb z entre z1 et z2
* @param[in] x1 abscisse 1
* @param[in] z1 ordonnée 1
* @param[in] x2 abscisse 2
* @param[in] z2 ordonnée 2
* @param[in] val11 valeur à la position [x1,z1]
* @param[in] val12 valeur à la position [x1,z2]
* @param[in] val21 valeur à la position [x2,z1]
* @param[in] val22 valeur à la position [x2,z2]
* @return la valeur à la position [xb,zb]
*/
float interp_linear2D(float xb, float zb,
                       float x1, float z1,
                       float x2, float z2,
                       float val11, float val12, float val21, float val22);
float interp_linear2D(const Vector2f& vb,
                       const Vector2f& v1,
                       const Vector2f& v2,
                       float val11, float val12, float val21, float val22);

/**
* @brief calcul d'une valeur sur une surface cosinusoïdale entre 4 positions
* @param[in] xb x entre x1 et x2
* @param[in] zb z entre z1 et z2
* @param[in] x1 abscisse 1
* @param[in] z1 ordonnée 1
* @param[in] x2 abscisse 2
* @param[in] z2 ordonnée 2
* @param[in] val11 valeur à la position [x1,z1]
* @param[in] val12 valeur à la position [x1,z2]
* @param[in] val21 valeur à la position [x2,z1]
* @param[in] val22 valeur à la position [x2,z2]
* @return la valeur à la position [xb,zb]
*/
float interp_cos2D(float xb, float zb,
                    float x1, float z1,
                    float x2, float z2,
                    float val11, float val12, float val21, float val22);
float interp_cos2D(const Vector2f& vb,
                    const Vector2f& v1,
                    const Vector2f& v2,
                    float val11, float val12, float val21, float val22);

/**
* @brief calcul d'une valeur sur une surface d'hermite entre 4 positions
* @param[in] xb x entre x1 et x2
* @param[in] zb z entre z1 et z2
* @param[in] x1 abscisse 1
* @param[in] z1 ordonnée 1
* @param[in] x2 abscisse 2
* @param[in] z2 ordonnée 2
* @param[in] val11 valeur à la position [x1,z1]
* @param[in] val12 valeur à la position [x1,z2]
* @param[in] val21 valeur à la position [x2,z1]
* @param[in] val22 valeur à la position [x2,z2]
* @return la valeur à la position abis
*/
float interp_hermite2D(float xb, float zb,
                        float x1, float z1,
                        float x2, float z2,
                        float val11, float val12, float val21, float val22);
float interp_hermite2D(const Vector2f& vb,
                        const Vector2f& v1,
                        const Vector2f& v2,
                        float val11, float val12, float val21, float val22);

}   //end namespace interp

#endif // INTERPOLATION_H
