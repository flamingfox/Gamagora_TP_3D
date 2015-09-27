#ifndef TERRAIN2_H
#define TERRAIN2_H

#include "./rendu/colorgradient.h"
#include "./parametres.h"
#include "./noisegenerator.h"
#include "box.h"

/**
 * @brief Classe virtuelle de terrain. Sert de modèle pour les classes files TerrainNoise et TerrainTab.
 */
class Terrain
{
public:
    Terrain();
    /**
     * @brief Constructeur de TerrainNoise.
     * @param[in] longueur Distance du terrain en metre sur l'axe y
     * @param[in] largeur Distance du terrain en metre sur l'axe x
     */
    Terrain(float longueur, float largeur);
    /**
     * @brief Constructeur de TerrainNoise.
     * @param[in] longueur Distance du terrain en metre sur l'axe y
     * @param[in] largeur Distance du terrain en metre sur l'axe x
     * @param[in] amplitude Amplitude Max que pourra atteindre la terrain.
     */
    Terrain(float longueur, float largeur, float amplitude);

    /**
     * @brief La boite englobante du terrain. (cf. Box)
     */
    Box box;

    /**
     * @brief Distance du terrain en metre sur l'axe y
     */
    float longueur;
    /**
     * @brief Distance du terrain en metre sur l'axe x.
     */
    float largeur;

    /**
     * @brief Conteneur de tableau de couleur. (cf. ColorGradient)
     */
    ColorGradient heatMapGradient;    // Used to create a nice array of different colors.


    /**
     * @brief hauteurMax Altitude max du terrain.
     */
    float hauteurMax;

    /**
     * @brief hauteurMin Altitude min du terrain.
     */
    float hauteurMin;


    /**
     * @brief Détermine la hauteur du terrain à la position \e x, \e y. \n
     * Regarde si la position est sur ou en dehors du terrain. \n
     * Passe le relai à getHauteurXY().
     * @param[in] x position en \e x de la hauteur à déterminer.
     * @param[in] y position en \e y de la hauteur à déterminer.
     * @return la hauteur du terrain à la position \e x, \e y. Si la position est hors map, la valeur sera HAUTEUR_HORS_MAP.
     */
    float getHauteur(float x, float y) const;
    /**
     * @brief Surchage de la methode getHauteur(float x, float y).
     * @param[in] pointXY Un point comprenant uniquement les axes \e x, \e y.
     * @return la hauteur du terrain au point \e pointXY.
     */
    float getHauteur(const Vector2f& pointXY) const;
    /**
     * @brief Surchage de la methode getHauteur(const Vector2f& pointXY).
     * @param[in] pointXYZ Un point comprenant uniquement les axes \e x, \e y, \e z.
     * @return la hauteur du terrain au point \e pointXYZ.
     */
    float getHauteur(const Vector3f& pointXYZ) const;


    /**
     * @brief Détermine la couleur de texture suivant une hauteur.
     * @param[out] r La valeur rouge de la couleur.
     * @param[out] g La valeur vert de la couleur.
     * @param[out] b La valeur bleu de la couleur.
     * @param[in] x Position en \e x du point de couleur à déterminer.
     * @param[in] y Position en \e y du point de couleur à déterminer.
     */
    void getColor(float& r, float& g, float& b, float x, float y) const;

    /**
     * @brief Test si un rayon traverse le terrain. \n
     * Si c'est le cas, la methode retournera la distance d'intersection la plus proche sur le terrain. (cf. Rayon) \n
     * Utilise la méthode intersect() de Box.
     * @param[in] rayon Un rayon quelconque. (cf. Rayon)
     * @param[out] coeffDistance coefficient de distance de l'impacte sur le terrain par rapport à l'origine du rayon. \n
     * Le coefficient est dependant de la norme du vecteur direction du rayon.
     * @return Le resultat du test. True = intersection, false = pas d'intersection.
     */
    bool intersect(const Rayon& rayon, float &coeffDistance) const;

    inline float getMinElevation() const {return hauteurMin;}
    inline float getMaxElevation() const {return hauteurMax;}

    /**
     * @brief Détermine la normal du terrain à la position \e x, \e y. \n
     * Regarde si les positions sont sur ou en dehors du terrain. \n
     * Passe le relai à getNormalXY().
     * @param[in] x position en \e x de la normal à déterminer.
     * @param[in] y position en \e y de la normal à déterminer.
     * @return la hauteur du terrain à la position \e x, \e y. Si la position est hors map, la valeur sera un vecteur nul.
     */
    Eigen::Vector3f getNormal(float x, float y) const;
    /**
     * @brief Surchage de la methode getNormal(float x, float y).
     * @param[in] pointXY Un point comprenant uniquement les axes \e x, \e y.
     * @return la normal du terrain au point \e pointXY.
     */
    Eigen::Vector3f getNormal(const Vector2f& pointXY) const;
    /**
     * @brief Surchage de la methode getHauteur(const Vector2f& pointXY).
     * @param[in] pointXYZ Un point comprenant uniquement les axes \e x, \e y, \e z.
     * @return la normal du terrain au point \e pointXYZ.
     */
    Eigen::Vector3f getNormal(const Vector3f& pointXYZ) const;

    /**
     * @brief Test si le point \e pointXYZ est sous le terrain.
     * @param[in] pointXYZ un point quelconque.
     * @return Le resultat du test. Si le point pointXYZ est en dehors de la carte, la valeur sera false.
     */
    bool inOut(const Eigen::Vector3f& pointXYZ) const;


    /**
     * @brief Methode virtuel pour déterminer la hauteur du terrain à un point. \n
     * A redéfinir dans les classes filles. (cf. TerrainNoise et TerrainTab)
     * @param[in] x la position en x du point.
     * @param[in] y la position en y du point.
     * @return la hauteur du terrain au point donné.
     */
    virtual float getHauteurXY(float x, float y) const = 0; //public car utilisé dans la classe Mesh

protected:

    /**
     * @brief Methode virtuel pour déterminer la normal du terrain à un point. \n
     * @param[in] x la position en x du point.
     * @param[in] y la position en y du point.
     * @return La normale à la position \e x,\e y.
     */
    virtual Eigen::Vector3f getNormalXY(float x, float y) const = 0;

    void translate2(const Vector3f& t);

    //void getColor2(float& r, float& g, float& b, float hauteur = 0, const Eigen::Vector3f& n = Eigen::Vector3f(0,0,1)) const;

    virtual float minElevation() const = 0;
    virtual float maxElevation() const = 0;

};

#endif // TERRAIN2_H
