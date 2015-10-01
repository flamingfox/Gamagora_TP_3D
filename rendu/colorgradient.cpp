#include "colorgradient.h"

  //-- Default constructor:
ColorGradient::ColorGradient()  {  createDefaultHeatMapGradient();  }

  //-- Inserts a new color point into its correct position:
void ColorGradient::addColorPoint(float red, float green, float blue, float value)
{
    for(unsigned int i=0; i<color.size(); i++)  {
        if(value < color[i].val) {
            color.insert(color.begin()+i, ColorPoint(red,green,blue, value));   //insérer la couleur avant un élément avec une valeur plus petite
            return;
        }
    }
    color.push_back(ColorPoint(red,green,blue, value));
}

  //-- Inserts a new color point into its correct position:
void ColorGradient::clearGradient() { color.clear(); }

  //-- Places a 5 color heapmap gradient into the "color" vector:
void ColorGradient::createDefaultHeatMapGradient()
{
    color.clear();
    color.push_back(ColorPoint(0.0/255.0, 173.0/255.0, 255.0/255.0,   0.0f));      // bleu.
    //color.push_back(ColorPoint(154.0/255.0, 205.0/255.0, 50.0/255.0,   0.0f));      // Herbe.
    //color.push_back(ColorPoint(139.0/255.0, 69.0/255.0, 19.0/255.0,   0.5f));     // Terre.
    //color.push_back(ColorPoint(139.0/255.0, 119.0/255.0, 101.0/255.0,   0.66f));      // Roche.
    //color.push_back(ColorPoint(255.0/255.0, 250.0/255.0, 250.0/255.0,   1.0f));      // Neige.
    color.push_back(ColorPoint(255.0/255.0, 0/255.0, 0.0/255.0,   1.0f));      // rouge.

}

  //-- Inputs a (value) between 0 and 1 and outputs the (red), (green) and (blue)
  //-- values representing that position in the gradient.
void ColorGradient::getColorAtValue(const float value, float &red, float &green, float &blue) const
{
    if(color.size()==0)
        return;

    for(int i=0; i< (int)color.size(); i++)
    {
        const ColorPoint &currC = color[i];
        if(value < currC.val)
        {
            const ColorPoint &prevC  = color[ std::max(0,i-1) ];    //attention de ne pas mettre i en unsigned
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
