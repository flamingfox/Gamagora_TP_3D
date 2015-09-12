#ifndef GENERATIONVEGETATION_H
#define GENERATIONVEGETATION_H

#include "mesh.h"
#include "arbre.h"
#include "terrain.h"

#include <Eigen/Core>
#include <time.h>

class GenerationVegetation
{
protected :
    Terrain terrain;
    std::vector<Arbre> especeArbre;
    std::vector<Arbre> arbreDispose;

public:
    GenerationVegetation();

    void initialisationArbreSurPlat(int nbForet, int nbArbreForet, int rayonForet);

    void iteration(int nbAnnees);

    Terrain getTerrain();
private :

    bool planterArbre(Arbre &arbre, float posX, float posY);
    void propagationArbre(Arbre &arbre);
};

#endif // GENERATIONVEGETATION_H
