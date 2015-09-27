#include "arbre.h"

#define ESPACE_VITAL_SAPIN 2
#define ESPACE_VITAL_SAULE 2
#define ESPACE_VITAL_CHARME 2

Arbre::Arbre(Arbre::TYPE_ARBRE type, int _age) : type(type), age(_age)
{
    switch (type) {
    case SAPIN :
        mesh = Mesh::arbreConique(Eigen::Vector3f(0,0,0), Eigen::Vector3f(0,1.5,0), 0.5,
                                  Eigen::Vector3f(0,1.5,0), Eigen::Vector3f(0,2.5,0), 1.0);
        mesh.Rotation(90,0,0);
        probabiliteReproduction = 0.3;
        proportionCroissement = 1.015;
        rayonEspaceVital = ESPACE_VITAL_SAPIN*pow(proportionCroissement, age);
        rayonReproduction = rayonEspaceVital + 20;
        ageMax = 150;
        break;
    case SAULE :
        mesh = Mesh::arbreSpherique(Eigen::Vector3f(0,0,0), Eigen::Vector3f(0,2,0), 0.5,
                                    Eigen::Vector3f(0,2,0), 1.0);
        mesh.Rotation(90,0,0);
        probabiliteReproduction = 0.3;
        proportionCroissement = 1.015;
        rayonEspaceVital = ESPACE_VITAL_SAULE*pow(proportionCroissement, age);
        rayonReproduction = rayonEspaceVital + 20;
        ageMax = 200;
        break;

    case CHARME :
        mesh = Mesh::arbreGalette(Eigen::Vector3f(0,0,0), Eigen::Vector3f(0,2,0), 0.5,
                                  Eigen::Vector3f(0,2,0), 0.7);
        mesh.Rotation(90,0,0);
        probabiliteReproduction = 0.3;
        proportionCroissement = 1.015;
        rayonEspaceVital = ESPACE_VITAL_CHARME*pow(proportionCroissement, age);
        rayonReproduction = rayonEspaceVital + 20;
        ageMax = 100;
        break;
    }
    majCaracteristique(true);
}

Arbre Arbre::reproduction(float ran)
{
    Arbre retour;

    if(ran >= 1-probabiliteReproduction){

        int posFilsX = (rand()%(int)(rayonReproduction*2) - rayonReproduction) + rayonEspaceVital+1;
        int posFilsY = (rand()%(int)(rayonReproduction*2) - rayonReproduction) + rayonEspaceVital+1;

        posFilsX += positionArbre(0);
        posFilsY += positionArbre(1);

        retour = Arbre(type, 1);
        retour.setPosition(posFilsX, posFilsY, 0);
    }

    return retour;
}

void Arbre::croissance(bool expensionAutorise)
{
    age++;
    majCaracteristique(expensionAutorise);
}

void Arbre::rescale()
{
    switch (type) {
    case SAPIN:
        setScale( rayonEspaceVital*(proportionCroissement) / ESPACE_VITAL_SAPIN);
        break;
    case SAULE:
        setScale( rayonEspaceVital*(proportionCroissement) / ESPACE_VITAL_SAULE);
        break;
    case CHARME:
        setScale( rayonEspaceVital*(proportionCroissement) / ESPACE_VITAL_CHARME);
        break;
    }
}

void Arbre::majCaracteristique(bool expensionAutoriser)
{
    if(expensionAutoriser){
        rayonEspaceVital *= proportionCroissement;
        rayonReproduction *= proportionCroissement;
        rescale();
    }
}


Mesh Arbre::getMesh() const
{
    Mesh retour = mesh;
    retour.rescale(scale);
    retour.Translation(positionArbre);
    return retour;
}

int Arbre::getRayonEspaceVital()
{
    return rayonEspaceVital;
}

Arbre::TYPE_ARBRE Arbre::getType()
{
    return type;
}

bool Arbre::isDead()
{
    if(age >= ageMax)
        return true;

    return false;
}

void Arbre::setPosition(Eigen::Vector3f _positionArbre)
{
    positionArbre = _positionArbre;
}

void Arbre::setPosition(float positionX, float positionY, float positionZ)
{
    setPosition(Eigen::Vector3f(positionX, positionY, positionZ));
}

void Arbre::setScale(float _scale)
{
    scale = _scale;
}

Eigen::Vector3f Arbre::getPosition()
{
    return positionArbre;
}
