#pragma once

#include <raylib.h>
#include <cmath>
#include <string>
#include "shape.h"
#include "mm/vec2.h"


class Rect : public Shape{
public:
    float h;           /*Rectangle height*/
    float w;           /*Rectangle width*/
    Rectangle rect;    /*Rectangle definition for Raylib*/

    Rect(const std::string& name,float x,float y,float h,float w,float speedX,float speedY,float* color);
    void Move(int width,int height) override;
    void Draw(const Font& font, int fontSize, const Color& fontColor, bool drawText) override;
};
