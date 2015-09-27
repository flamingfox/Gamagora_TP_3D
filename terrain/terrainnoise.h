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
     * @brief Récupere la hauteur du terrain à un point donné. \n
     * Redéfinition de la methode. (cf. Terrain)
     * @param[in] x abscisse du terrain (entre 0 et 1).
     * @param[in] y ordonnée du terrain (entre 0 et 1).
     * @return la hauteur du terrain au point donné.
     */
    float getHauteurXY(float x, float y) const;

    /**
     * @brief Récupération d'une valeur de noise suivant les paramétres donnés.
     * @param[in] amplitude Amplitude max que pourra atteindre le noise.
     * @param[in] periode Distance en metre pour que le noise atteigne un cycle.
     * @param[in] x abscisse du terrain (entre 0 et 1).
     * @param[in] y ordonnée du terrain (entre 0 et 1).
     * @return La valeur générée par le noise.
     */
    float noise(int amplitude, float periode, float x, float y)const;

    /**
     * @brief Application sur une valeur de hauteur d'un ridge (seuil avec rebond) suivant un seuil.
     * @param[in] hauteur La hauteur à soumettre au ridge.
     * @param[in] seuil Le seuil d'application du ridge. Si le seuil est au-dessus du maximum atteignable par la hauteur, aucun ridge ne pourra être appliqué dessus.
     * @return La hauteur après avoir passé le ridge.
     */
    float ridge(float hauteur, float seuil)const;

    /**
     * @brief Calcul la normale d'un point sur le terrain.
     * @param[in] x abscisse du terrain (entre 0 et 1).
     * @param[in] y ordonnée du terrain (entre 0 et 1).
     * @return la hauteur du terrain à ses coordonnées x, y.
     */
    Eigen::Vector3f getNormalXY(float x, float y) const;

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

};

#endif // TERRAINNOISE_H
