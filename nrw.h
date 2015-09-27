#ifndef NRW_H
#define NRW_H

#include "noisegenerator.h"

/**fonction pour noise, ridge, warp et toutes fonction sur le terrain qui n'ont pas besoin de connaitre les caractéristiques du terrain*/
namespace nrw
{
float noise(int amplitude, float periode, float x, float y);

/**
 * @param hauteur hauteur que l'on veut modifier par un ridge
 * @param seuil hauteur max et de découpe du terrain
 * @return la nouvelle hauteur < seuil
*/
float ridge(float hauteur, float seuil);

///
/// \brief Terrain::attenuation
/// \param h valeur que l'on souhaite atténuer
/// \param min seuil minimum à partir duquel la fonctin renvoie un résulttat > à 0
/// \param max seuil à partir duquel la fonction renvoie toujours 1
/// \return return a factor between 0 and 1 depending on the elevation
///
float attenuation(float h, float min, float max);



}

#endif // NRW_H
