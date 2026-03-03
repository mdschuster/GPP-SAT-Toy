#include <string>
#include "shape.h"

/**
 * Initializes a Shape object
 * @param name    Name/ID of object
 * @param x       X position
 * @param y       Y position
 * @param h       Height
 * @param w       Width
 * @param speedX  X speed
 * @param speedY  Y speed
 * @param color   Fill color
 */
Shape::Shape(const std::string& name, float x, float y,float speedX, float speedY, float* color){
    this->name=name;
    this->speedX=speedX;
    this->speedY=speedY;
    this->lineColor=color;
    this->color=new float[3];
    this->x=x;
    this->y=y;
    scaleFactor=1.0f;
    active=true;
    std::copy(lineColor, lineColor + 3, this->color);
}
/**
 * Empty Initialization
 */
Shape::Shape(){};
/**
 * Destructor that will free color arrays
 */
Shape::~Shape(){delete color; delete lineColor;};
