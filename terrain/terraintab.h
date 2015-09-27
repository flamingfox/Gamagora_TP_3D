#ifndef TERRAINTAB_H
#define TERRAINTAB_H

#include <QImage>
#include "interpolation.h"
#include "terrain.h"

/**
* @brief Classe fille de Terrain. Elle s'appuis sur l'utilisation d'un image HighMap pour déterminer la forme du terrain.
*/
class TerrainTab: public Terrain
{
public:
    TerrainTab():   height(0),  width(0),   amplitude(0),   hauteurMin(0),  hauteurMax(0){}

    /**
     * @param[in] img Image HighMap contenant le relief du terrain.
     * @param[in] longueur Distance du terrain en metre sur l'axe y.
     * @param[in] largeur Distance du terrain en metre sur l'axe x.
     * @param[in] amplitude Amplitude Max que pourra atteindre la terrain. 255 sur un pixel correspondra à cette valeur. Par défaut, la valeur est de 1.
     */
    TerrainTab(const QImage& img, float longueur, float largeur, float amplitude = 1.0f);

    /**
     * @param[in] img Image HighMap contenant le relief du terrain.
     * @param[in] _nbHeight Nombre de points composant l'axe y du terrain. Ce nombre détermine la précision de la topographie du terrain sur cette axe.
     * @param[in] _nbWidth Nombre de points composant l'axe x du terrain. Ce nombre détermine la précision de la topographie du terrain sur cette axe.
     * @param[in] longueur Distance du terrain en metre sur l'axe y.
     * @param[in] largeur Distance du terrain en metre sur l'axe x.
     * @param[in] _amplitude Amplitude Max que pourra atteindre la terrain. 255 sur un pixel correspondra à cette valeur. Par défaut, la valeur est de 1.
     */
    TerrainTab(const QImage& img, int _nbHeight, int _nbWidth, float longueur, float largeur, float _amplitude = 1.0f);

    /**
     * @brief Constructeur par copie.
     * @param[in] Le terrain à copier.
     */
    TerrainTab(const TerrainTab& copy);

    ~TerrainTab();

private:

    /**
     * @brief height Hauteur de l'image HighMap.
     */
    int height;
    /**
     * @brief width Largeur de l'image HighMap.
     */
    int width;

    /**
     * @brief amplitude Amplitude max attegnable par le terrain.
     */
    float amplitude;

    /**
     * @brief grille
     */
    float *grille = nullptr;
    /**
     * @brief grille2d
     */
    float **grille2d = nullptr;

    /**
     * @brief hauteurMax Altitude max du terrain.
     */
    float hauteurMax;

    /**
     * @brief hauteurMin Altitude min du terrain.
     */
    float hauteurMin;

    /**
     * @brief Récupere la hauteur du terrain à un point donné. \n
     * Redéfinition de la methode. (cf. Terrain)
     * @param[in] x abscisse du terrain (entre 0 et 1).
     * @param[in] y ordonnée du terrain (entre 0 et 1).
     * @return la hauteur du terrain au point donné.
     */
    float getHauteurXY(float x, float y) const;

    /**
     * @brief Calcul la normale d'un point sur le terrain.
     * @param[in] x abscisse du terrain (entre 0 et 1).
     * @param[in] y ordonnée du terrain (entre 0 et 1).
     * @return la hauteur du terrain à ses coordonnées x, y.
     */
    Eigen::Vector3f getNormalXY(float x, float y) const;

    /**
     * @brief Initialise et remplis la grille topographique avec l'image donnée dans le constructeur. Deplus les indications complémentaire : _nbHeight et _nbWidth sont utilisées dans le processus.
     */
    void initGrille();

    /**
     * @brief Initialise et remplis la grille topographique suivant l'image données en paramètre. La grille aura le même precision que l'image donnée.
     * @param[in] L'image HighMap contenant le relief du terrain.
     */
    void simpleInitImage(const QImage& img);

    /**
     * @brief Récupere la hauteur du terrain à un point donné. Le point doit correspondre à un point existant sur la carte. Aucun approximation ne sera faite.
     * @param[in] x la position en x du point.
     * @param[in] y la position en y du point.
     * @return la hauteur du terrain au point donné.
     */
    inline float get(int x, int y) const;

    /**
     * @brief Détermine l'élévation minimum du terrain et ajuste #hauteurMin.
     */
    inline void updateMinElevation();
    /**
     * @brief Détermine l'élévation maximum du terrain et ajuste #hauteurMax.
     */
    inline void updateMaxElevation();
    /**
     * @brief Détermine l'élévation minimum et maximum du terrain et ajuste #hauteurMin, #hauteurMax.
     */
    inline void updateElevation();


    /**
     * @return L'élévation minimim du terrain.
     */
    float minElevation() const;

    /**
     * @return L'élévation maximum du terrain.
     */
    float maxElevation() const;
};


/*************************************************************************************/
//fonction inline




inline void TerrainTab::updateMinElevation()
{
    hauteurMin = grille[0];
#pragma omp parallel for schedule(dynamic,1)
    for(int i = 1; i < height*width; i++){
        float hauteur = grille[i];
        if(hauteur<hauteurMin)    hauteurMin=hauteur;
    }
}

inline void TerrainTab::updateMaxElevation()
{
    hauteurMax = grille[0];
#pragma omp parallel for schedule(dynamic,1)
    for(int i = 1; i < height*width; i++){
        float hauteur = grille[i];
        if(hauteur>hauteurMax)    hauteurMax = hauteur;
    }
}


inline void TerrainTab::updateElevation()
{
    hauteurMin = grille[0];
    hauteurMax = grille[0];
    #pragma omp parallel for schedule(dynamic,1)
    for(int i = 1; i < height*width; i++){
        float hauteur = grille[i];
        if(hauteur<hauteurMin)    hauteurMin=hauteur;
        if(hauteur>hauteurMax)    hauteurMax = hauteur;
    }
}


#endif // TERRAINTAB_H
