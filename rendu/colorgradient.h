#ifndef COLORGRADIENT_H
#define COLORGRADIENT_H

#include <vector>

/**
 * @brief Class containing multiple color and perform a gradient on this color.
 */
class ColorGradient
{
private:

    /**
    * @brief Internal class used to store colors at different points in the gradient.
    */
    struct ColorPoint
    {
        /** @brief r red value of our color. */
        float r;
        /** @brief g green value of our color. */
        float g;
        /** @brief b blue value of our color. */
        float b;

        /**
         * @brief Position of our color along the gradient (between 0 and 1).
         */
        float val;

        ColorPoint(float red, float green, float blue, float value)
            : r(red), g(green), b(blue), val(value) {}
    };

    /**
     * @brief An array of color points in ascending value.
     */
    std::vector<ColorPoint> color;

public:
    //-- Default constructor:
    ColorGradient();

    /**
     * @brief Inserts a new color point into its correct position.
     * @param[in] red red value of the color.
     * @param[in] green green value of the color.
     * @param[in] blue blue value of the color.
     * @param[in] value position along the gradient.
     */
    void addColorPoint(float red, float green, float blue, float value);

    /**
     * @brief Inserts a new color point into its correct position.
     */
    void clearGradient();

    /**
     * @brief Places a 5 color heapmap gradient into the "color" vector.
     */
    void createDefaultHeatMapGradient();

    /**
     * @brief values representing that position in the gradient.
     * @param[in] value a value between 0 and 1.
     * @param[out] red red value of the color.
     * @param[out] green green value of the color.
     * @param[out] blue blue value of the color.
     */
    void getColorAtValue(const float value, float &red, float &green, float &blue) const;
};

#endif // COLORGRADIENT_H
