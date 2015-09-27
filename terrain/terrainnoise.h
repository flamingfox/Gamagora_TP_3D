#ifndef TERRAINNOISE_H
#define TERRAINNOISE_H

#include "parametres.h"
#include "terrain2.h"
#include "noisegenerator.h"

/**
 * @brief Classe fille de Terrain2. Elle s'appuis sur l'utilisation de noise pour déterminer la forme du terrain.
 *
 */
class TerrainNoise: public Terrain2
{
public:
    TerrainNoise();
    /**
     * @brief Constructeur de TerrainNoise.
     * @param _longueur Distance du terrain en metre sur l'axe y
     * @param _largeur Distance du terrain en metre sur l'axe x
     */
    TerrainNoise(int _longueur, int _largeur);

protected:
    /**
     * @brief Récupere la hauteur du terrain à un point donné. \n
     * Redéfinition de la methode. (cf. Terrain2)
     * @param x la position en x du point.
     * @param y la position en y du point.
     * @return la hauteur du terrain au point donné.
     */
    float getHauteurXY(float x, float y) const;

    /**
     * @brief Récupération d'une valeur de noise suivant les paramétres donnés.
     * @param amplitude Amplitude max que pourra atteindre le noise.
     * @param periode Distance en metre pour que le noise atteigne un cycle.
     * @param x Position en x d'un point donné au noise.
     * @param y Position en y d'un point donné au noise.
     * @return La valeur générée par le noise.
     */
    float noise(int amplitude, float periode, float x, float y)const;

    /**
     * @brief Application sur une valeur de hauteur d'un ridge (seuil avec rebond) suivant un seuil.
     * @param hauteur La hauteur à soumettre au ridge.
     * @param seuil Le seuil d'application du ridge. Si le seuil est au-dessus du maximum atteignable par la hauteur, aucun ridge ne pourra être appliqué dessus.
     * @return La hauteur après avoir passé le ridge.
     */
    float ridge(float hauteur, float seuil)const;
};

#endif // TERRAINNOISE_H
