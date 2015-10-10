#ifndef TERRAINNOISE_H
#define TERRAINNOISE_H

#include "parametres.h"
#include "terrain.h"
#include "noisegenerator.h"
#include "nrw.h"

/**
 * @brief Classe fille de Terrain. Elle s'appuis sur l'utilisation de noise pour déterminer la forme du terrain.
 *
 */
class TerrainNoise: public Terrain
{
public:
    TerrainNoise();
    /**
     * @brief Constructeur de TerrainNoise.
     * @param[in] _longueur Distance du terrain en metre sur l'axe y
     * @param[in] _largeur Distance du terrain en metre sur l'axe x
     */
    TerrainNoise(int _longueur, int _largeur);

protected:

    /**
     * @brief Détermine la hauteur du terrain à la position \e x, \e y. \n
     * Regarde si la position est sur ou en dehors du terrain.
     * @param[in] x position en \e x de la hauteur à déterminer.
     * @param[in] y position en \e y de la hauteur à déterminer.
     * @return la hauteur du terrain à la position \e x, \e y. Si la position est hors map, la valeur sera HAUTEUR_HORS_MAP.
     */
    float getHauteur(float x, float y) const;

    /**
     * @brief Calcul la normale d'un point sur le terrain.
     * @param[in] x abscisse du terrain (entre 0 et largeur).
     * @param[in] y ordonnée du terrain (entre 0 et longueur).
     * @return la hauteur du terrain à ses coordonnées x, y.
     */
    Eigen::Vector3f getNormal(float x, float y) const;

    /**
     * @brief Calcul l'élévation minimum du terrain.
     * @return L'élévation minimim du terrain.
     */
    float minElevation() const;

    /**
     * @brief Calcul l'élévation maximum du terrain.
     * @return L'élévation maximum du terrain.
     */
    float maxElevation() const;

private:

    inline float getHauteur2(float x, float y) const;
};

#endif // TERRAINNOISE_H
