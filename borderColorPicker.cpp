#include "borderColorPicker.h"

borderColorPicker::borderColorPicker(unsigned int borderSize, HSLAPixel fillColor, PNG &img, double tolerance)
{
    /* your code here */
    this->borderSize = borderSize;
    this->fillColor = fillColor;
    this->img = img;
    this->tolerance = tolerance;
}

HSLAPixel borderColorPicker::operator()(point p)
{
    int w = img.width();
    int h = img.height();

    for (int i = 0; i<w; i++) {
        int x_dist = p.x - i;
        for(int j = 0; j<h; j++) {
            int y_dist = p.y - j;
            int distSq = x_dist * x_dist + y_dist * y_dist;
            if ((distSq <= (borderSize * borderSize)) &&
            (p.c.color.dist(*img.getPixel(i, j)) > tolerance)) {
                return fillColor;
            }
        }
    }

    if ((p.x < borderSize || w <= borderSize + p.x) || (p.y < borderSize || h <= borderSize + p.y)) {
        return fillColor;
    } else {
        return *img.getPixel(p.x,p.y);
    }
}
