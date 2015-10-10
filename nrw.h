#ifndef NRW_H
#define NRW_H

#include "noisegenerator.h"

/**
 * fonction pour noise, ridge, warp et toutes fonction sur le terrain qui n'ont pas besoin de connaitre les caractéristiques du terrain
 */
namespace nrw
{
/**
 * @brief Récupération d'une valeur de noise suivant les paramétres donnés.
 * @param[in] amplitude Amplitude max que pourra atteindre le noise.
 * @param[in] periode Distance en metre pour que le noise atteigne un cycle.
 * @param[in] x abscisse du terrain (entre 0 et 1).
 * @param[in] y ordonnée du terrain (entre 0 et 1).
 * @return La valeur générée par le noise.
 */
float noise(int amplitude, float periode, float x, float y);

/**
 * @param hauteur hauteur que l'on veut modifier par un ridge
 * @param seuil hauteur max et de découpe du terrain
 * @return la nouvelle hauteur < seuil
*/
float ridge(float hauteur, float seuil);

void warp(float& x, float&y, float amplitude, float periode);
void warp(float x, float y, float& x2, float&y2, float amplitude, float periode);


/**
 * @brief Terrain::attenuation
 * @param[in] h valeur que l'on souhaite atténuer
 * @param[in] min seuil minimum à partir duquel la fonctin renvoie un résulttat > à 0
 * @param[in] max seuil à partir duquel la fonction renvoie toujours 1
 * @return Un coefficient entre 0 et 1 dépendant de l'élévation
 */
float attenuation(float h, float min, float max);

}

#endif // NRW_H
