#ifndef TERRAINTAB_H
#define TERRAINTAB_H

#include <QImage>
#include "interpolation.h"
#include "terrain2.h"

/**
 * @brief Classe fille de Terrain2. Elle s'appuis sur l'utilisation d'un image HighMap pour déterminer la forme du terrain.
 */
class TerrainTab: Terrain2
{
public:
    TerrainTab():   height(0),  width(0),   amplitude(0)    {}

    /**
     * @param img Image HighMap contenant le relief du terrain.
     * @param longueur Distance du terrain en metre sur l'axe y.
     * @param largeur Distance du terrain en metre sur l'axe x.
     * @param amplitude Amplitude Max que pourra atteindre la terrain. 255 sur un pixel correspondra à cette valeur. Par défaut, la valeur est de 1.
     */
    TerrainTab(const QImage& img, float longueur, float largeur, float amplitude = 1.0f);

    /**
     * @param img Image HighMap contenant le relief du terrain.
     * @param _nbHeight Nombre de points composant l'axe y du terrain. Ce nombre détermine la précision de la topographie du terrain sur cette axe.
     * @param _nbWidth Nombre de points composant l'axe x du terrain. Ce nombre détermine la précision de la topographie du terrain sur cette axe.
     * @param longueur Distance du terrain en metre sur l'axe y.
     * @param largeur Distance du terrain en metre sur l'axe x.
     * @param _amplitude Amplitude Max que pourra atteindre la terrain. 255 sur un pixel correspondra à cette valeur. Par défaut, la valeur est de 1.
     */
    TerrainTab(const QImage& img, int _nbHeight, int _nbWidth, float longueur, float largeur, float _amplitude = 1.0f);

    /**
     * @brief Constructeur par copie.
     * @param Le terrain à copier.
     */
    TerrainTab(const TerrainTab& copy);

    ~TerrainTab();

private:

    /**
     * @brief height Distance du terrain en metre sur l'axe y.
     */
    int height;
    /**
     * @brief width Distance du terrain en metre sur l'axe x.
     */
    int width;

    /**
     * @brief grille
     */
    float *grille = nullptr;
    /**
     * @brief grille2d
     */
    float **grille2d = nullptr;

    /**
     * @brief amplitude Amplitude max attegnable par le terrain.
     */
    float amplitude;

    /**
     * @brief Récupere la hauteur du terrain à un point donné. Si le point donné n'existe physiquement pas sur le terrain, une approximation sera faite grace au points alentours.
     * @param x la position en x du point.
     * @param y la position en y du point.
     * @return la hauteur du terrain au point donné.
     */
    float getHauteurXY(float x, float y) const;

    /**
     * @brief Initialise et remplis la grille topographique avec l'image donnée dans le constructeur. Deplus les indications complémentaire : _nbHeight et _nbWidth sont utilisées dans le processus.
     */
    void initGrille();

    /**
     * @brief Initialise et remplis la grille topographique suivant l'image données en paramètre. La grille aura le même precision que l'image donnée.
     * @param L'image HighMap contenant le relief du terrain.
     */
    void simpleInitImage(const QImage& img);

    /**
     * @brief Récupere la hauteur du terrain à un point donné. Le point doit correspondre à un point existant sur la carte. Aucun approximation ne sera faite.
     * @param x la position en x du point.
     * @param y la position en y du point.
     * @return la hauteur du terrain au point donné.
     */
    inline float get(int x, int y) const;
};

#endif // TERRAINTAB_H
