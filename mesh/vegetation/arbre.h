#ifndef ARBRE_H
#define ARBRE_H

#include <Eigen/Core>
#include "mesh.h"
#include "arbre.h"

class Arbre
{

public:
    enum TYPE_ARBRE {VIDE, SAPIN, SAULE, CHARME};

    Arbre() : type(VIDE) {};
    Arbre(TYPE_ARBRE type, int _age);

    Arbre reproduction(float ran);

    void croissance(bool expensionAutorise);
    void rescale();
    void majCaracteristique(bool expensionAutoriser);

    void setPosition(Eigen::Vector3f _positionArbre);
    void setPosition(float positionX, float positionY, float positionZ);

    void setScale(float _scale);
    void setAge(int _age);

    Eigen::Vector3f getPosition();
    Mesh getMesh() const;
    int getRayonEspaceVital();
    TYPE_ARBRE getType();

    bool isDead();

protected :
    Eigen::Vector3f positionArbre;
    TYPE_ARBRE type;
    Mesh mesh;
    int age;
    int ageMax;
    float rayonEspaceVital; //non ce n'es pas nazi
    float rayonReproduction;
    float probabiliteReproduction;
    float proportionCroissement;

private:
    float scale;
};

#endif // ARBRE_H
