#include "fadeColorPicker.h"

fadeColorPicker::fadeColorPicker(double fade)
{
    /* your code here */
    fadeFactor = fade;
}

HSLAPixel fadeColorPicker::operator()(point p)
{
    /* your code here */
    int x_dist = p.x - p.c.x;
    int y_dist = p.y - p.c.y;
    double dist = sqrt((x_dist * x_dist) + (y_dist * y_dist));
    HSLAPixel ret = p.c.color;
    ret.l = p.c.color.l * pow(fadeFactor, dist);
    return ret;
}
