#pragma once
#include "raylib.h"
#include "mm/vec2.h"
#include <vector>
#include <string>

class Shape{
public:
    std::string name;             /*Name/ID of shape*/
    std::vector<Vec2> vertices;   /*Vector of shape vertices*/
    float scaleFactor;            /*Shape scale controlled in UI*/
    float speedX;                 /*X speed*/
    float speedY;                 /*Y speed*/
    float x;                      /*X position*/
    float y;                      /*Y position*/
    float* color;                 /*Fill color RGB_F32*/
    float* lineColor;             /*Line color RGB_F32*/
    float angle;                  /*Current rotation angle*/
    float angularVelocity=0.1f;   /*Rotation speed*/
    bool active;                  /*Active in the simulation flag*/
    Shape(const std::string& name, float x, float y,float speedX, float speedY, float* color);
    Shape();
    virtual ~Shape();
    /**
     * Moves and rotates object, including bouncing off of screen edges
     * @param width  Screen width
     * @param height Screen Height
     */
    virtual void Move(int width,int height)=0;
    /**
     * Draws filled version of the object with Name/ID in the center and vertices as circles. If it is currently
     * colliding with another object, the outline will be drawn in a different color.
     * @param font      Font name
     * @param fontSize  Font size
     * @param fontColor Font color
     * @param drawText  Flag to draw text
     */
    virtual void Draw(const Font& font, int fontSize, const Color& fontColor, bool drawText)=0;
};
