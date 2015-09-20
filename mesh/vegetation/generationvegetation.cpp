#include "generationvegetation.h"

GenerationVegetation::GenerationVegetation()
{
    especeArbre.push_back(Arbre(Arbre::SAULE,0));
    especeArbre.push_back(Arbre(Arbre::SAPIN,0));
    especeArbre.push_back(Arbre(Arbre::CHARME,0));
}

void GenerationVegetation::initialisationArbreSurPlat(int nbForet, int nbArbreForet, int rayonForet)
{
    terrain = Terrain(Terrain::CRETE_NOISE, 150, 150, 2, 0.5);
    terrain.rescale(10);
    terrain.save("terrain.obj");

    std::vector<Eigen::Vector2f> listCentreForet;
    for(int i = 0; i < nbForet; i++ ){
        int choix = rand()%(especeArbre.size());

        Arbre arbre = especeArbre.at(choix);

        int centreXForet = rand()%( (int)terrain.getDimension()(0) - rayonForet*2) + rayonForet;
        int centreYForet = rand()%( (int)terrain.getDimension()(1) - rayonForet*2) + rayonForet;

        bool foretOk = true;

        for(int nbForet=0; nbForet < listCentreForet.size(); nbForet++){
            Eigen::Vector2f tmp = listCentreForet.at(nbForet);

            int deltaX1 = abs( (centreXForet + rayonForet) - (tmp(0) - rayonForet));
            int deltaY1 = abs( (centreYForet + rayonForet) - (tmp(1) - rayonForet));
            int deltaX2 = abs( (centreXForet - rayonForet) - (tmp(0) + rayonForet));
            int deltaY2 = abs( (centreYForet - rayonForet) - (tmp(1) + rayonForet));

            if(deltaX1 <= 0 || deltaX2 <= 0 || deltaY1 <= 0 || deltaY2 <= 0 ){
                i--;
                foretOk = false;
                break;
            }
        }

        if(foretOk){
            for(int nbArbre = 0; nbArbre < nbArbreForet; nbArbre++){
                int posArbreX = ( (rand()%(rayonForet*2) )- rayonForet) + centreXForet;
                int posArbreY = ( (rand()%(rayonForet*2) )- rayonForet) + centreYForet;

                Arbre arbre2 = Arbre(arbre.getType(), rand()%60+10);

                planterArbre(arbre2, posArbreX, posArbreY);
            }
        }
    }
}

void GenerationVegetation::iteration(int nbAnnees)
{
    for(int i=0; i<nbAnnees;i++){
        for(int indiceArbre=0; indiceArbre<arbreDispose.size(); indiceArbre++){
            Arbre arbre = arbreDispose.at(indiceArbre);
            arbre.croissance(true);
            propagationArbre(arbre);

            if(arbre.isDead())
                arbreDispose.erase(arbreDispose.begin()+i);
            else
                arbreDispose.at(indiceArbre) = arbre;
        }

    }
}

Terrain GenerationVegetation::getTerrain()
{
    Terrain t = terrain;

    for(int i=0; i< arbreDispose.size(); i++){
        t.merge(arbreDispose.at(i).getMesh());
    }

    return t;
}

bool GenerationVegetation::planterArbre(Arbre &arbre, float posX, float posY)
{
    bool plantationAutorisation = false;

    if(posX > 0 && posX < terrain.getDimension()(0) && posY > 0 && posY < terrain.getDimension()(1)){

        plantationAutorisation = true;

        for(int i=0; i < arbreDispose.size(); i++){
            Arbre tmp = arbreDispose.at(i);
            int deltaX = abs( posX - tmp.getPosition()(0) );
            int deltaY = abs( posY - tmp.getPosition()(1) );

            if(deltaX <= tmp.getRayonEspaceVital() || deltaY <= tmp.getRayonEspaceVital()){
                plantationAutorisation = false;
                break;
            }
        }

        if(plantationAutorisation){
            Arbre nn = arbre;
            arbre.setPosition(posX, posY, terrain.getHauteur(posX, posY) );
            arbreDispose.push_back(arbre);
        }
    }

    return plantationAutorisation;
}

void GenerationVegetation::propagationArbre(Arbre &arbre)
{

    float ran = ((float)(rand()%100)) / 100;
    Arbre fils = arbre.reproduction(ran);

    if(fils.getType() != Arbre::VIDE){
        planterArbre(fils, fils.getPosition()(0), fils.getPosition()(1));
    }
}
