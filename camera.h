#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>

#include <Eigen/Core>
#include <QList>
#include <rayon.h>
#include <terrain.h>
#include "float.h"  //FLT_MIN FLT_MAX
#include <QColor>
#include <QDebug>
#include "rendu.h"
#include <QLinearGradient>
#include <QBrush>

using namespace Eigen;

class ColorGradient
{
private:
  struct ColorPoint  // Internal class used to store colors at different points in the gradient.
  {
    float r,g,b;      // Red, green and blue values of our color.
    float val;        // Position of our color along the gradient (between 0 and 1).
    ColorPoint(float red, float green, float blue, float value)
      : r(red), g(green), b(blue), val(value) {}
  };
  std::vector<ColorPoint> color;      // An array of color points in ascending value.

public:
  //-- Default constructor:
  ColorGradient()  {  createDefaultHeatMapGradient();  }

  //-- Inserts a new color point into its correct position:
  void addColorPoint(float red, float green, float blue, float value)
  {
    for(int i=0; i<color.size(); i++)  {
      if(value < color[i].val) {
        color.insert(color.begin()+i, ColorPoint(red,green,blue, value));
        return;  }}
    color.push_back(ColorPoint(red,green,blue, value));
  }

  //-- Inserts a new color point into its correct position:
  void clearGradient() { color.clear(); }

  //-- Places a 5 color heapmap gradient into the "color" vector:
  void createDefaultHeatMapGradient()
  {
    color.clear();
    color.push_back(ColorPoint(154.0/255.0, 205.0/255.0, 50.0/255.0,   0.0f));      // Herbe.
    color.push_back(ColorPoint(139.0/255.0, 69.0/255.0, 19.0/255.0,   0.5f));     // Terre.
    //color.push_back(ColorPoint(139.0/255.0, 119.0/255.0, 101.0/255.0,   0.66f));      // Roche.
    color.push_back(ColorPoint(255.0/255.0, 250.0/255.0, 250.0/255.0,   1.0f));      // Neige.
  }

  //-- Inputs a (value) between 0 and 1 and outputs the (red), (green) and (blue)
  //-- values representing that position in the gradient.
  void getColorAtValue(const float value, float &red, float &green, float &blue)
  {
    if(color.size()==0)
      return;

    for(int i=0; i<color.size(); i++)
    {
      ColorPoint &currC = color[i];
      if(value < currC.val)
      {
        ColorPoint &prevC  = color[ std::max(0,i-1) ];
        float valueDiff    = (prevC.val - currC.val);
        float fractBetween = (valueDiff==0) ? 0 : (value - currC.val) / valueDiff;
        red   = (prevC.r - currC.r)*fractBetween + currC.r;
        green = (prevC.g - currC.g)*fractBetween + currC.g;
        blue  = (prevC.b - currC.b)*fractBetween + currC.b;
        return;
      }
    }
    red   = color.back().r;
    green = color.back().g;
    blue  = color.back().b;
    return;
  }
};



class Camera
{
protected:
    Vector3f _origine; //position de la camera
    Vector3f _u, _v, _w;
    int _lu, _lv;
    float _lw; //lv et lw constants    exemple : lu = 16.0 / 20.0
                         //                                lv = 9.0 / 20.0
    std::vector<Terrain*> _t;//liste des objets dans la scene
    // un vecteur u, un vecteur v et un vecteur w  (u,v,w)
    // une distance dw
    // une distance lu et lv
    QColor colors[5]={QColor(154,205,50),QColor(139,69,19),QColor(205,183,158),QColor(255,250,250),QColor(255,250,250)};
    QColor renderHors();
    QColor render(const bool toucheoupas, const Eigen::Vector3f& pointImpact, const Terrain& objleplusproche, const Rayon& ray);

    ColorGradient heatMapGradient;    // Used to create a nice array of different colors.

public:
    Camera();
    Camera(const Vector3f& pOr, const Vector3f& vAt, int l, int h, const std::vector<Terrain*>& listTerrain);
    Camera(const Vector3f& pOr, const Vector3f& vAtUnit, const float& distance, int l, int h, const std::vector<Terrain*>& listTerrain);
    //Camera(const Vector3f& pOr, const Vector3f& vAtUnit, const float& distance, int l, int h, const std::vector<Terrain*>& listTerrain);

    Vector3f vecScreen(int i, int j) const;
    Vector3f pointScreen(int i, int j) const;

    bool SetPixel(QImage *img, int x, int y, const QColor& color);//permet de mettre une couleur au pixel de coordonnées x,y

    int getrouge(const QRgb& couleur);
    bool rendu();
    QImage* antialiasing(QImage *img);
    QImage generateImage(int largeur, int hauteur);//fonction de test à ne pas utiliser


};
#endif // CAMERA_H
