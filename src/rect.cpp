#include "rect.h"
#include <string>
#include <cmath>
#include <algorithm>

/**
 * Creates a Rectangle object
 * @param name    Name/ID of object
 * @param x       X position
 * @param y       Y position
 * @param h       Height
 * @param w       Width
 * @param speedX  X speed
 * @param speedY  Y speed
 * @param color   Fill color
 */
Rect::Rect(const std::string &name, float x, float y, float h, float w, float speedX, float speedY, float *color)
        : Shape(name, x, y, speedX, speedY, color) {
    this->h = h;
    this->w = w;
    rect = {x, y, w * scaleFactor, h * scaleFactor};
    float hh = rect.height / 2.0f;
    float hw = rect.width / 2.0f;
    angle = 0;
    float currentAngle = DEG2RAD * angle;
    vertices = {Vec2{x - hw * cosf(currentAngle) + hh * sinf(currentAngle),
                     y - hw * sinf(currentAngle) - hh * cosf(currentAngle)},
                Vec2{x + hw * cosf(currentAngle) + hh * sinf(currentAngle),
                     y + hw * sinf(currentAngle) - hh * cosf(currentAngle)},
                Vec2{x + hw * cosf(currentAngle) - hh * sinf(currentAngle),
                     y + hw * sinf(currentAngle) + hh * cosf(currentAngle)},
                Vec2{x - hw * cosf(currentAngle) - hh * sinf(currentAngle),
                     y - hw * sinf(currentAngle) + hh * cosf(currentAngle)}};
}

/**
 * Moves and rotates object, including bouncing off of screen edges
 * @param width  Screen width
 * @param height Screen Height
 */
void Rect::Move(int width, int height) {
    x += speedX;
    y += speedY;
    angle += angularVelocity;
    rect.x = x;
    rect.y = y;
    rect.width = w * scaleFactor;
    rect.height = h * scaleFactor;
    //update vertices
    float hh = rect.height / 2.0f;
    float hw = rect.width / 2.0f;
    float currentAngle = DEG2RAD * angle;
    vertices = {Vec2{x - hw * cosf(currentAngle) + hh * sinf(currentAngle),
                     y - hw * sinf(currentAngle) - hh * cosf(currentAngle)},
                Vec2{x + hw * cosf(currentAngle) + hh * sinf(currentAngle),
                     y + hw * sinf(currentAngle) - hh * cosf(currentAngle)},
                Vec2{x + hw * cosf(currentAngle) - hh * sinf(currentAngle),
                     y + hw * sinf(currentAngle) + hh * cosf(currentAngle)},
                Vec2{x - hw * cosf(currentAngle) - hh * sinf(currentAngle),
                     y - hw * sinf(currentAngle) + hh * cosf(currentAngle)}};

    //do collision tests using extent of rectangle
    float ex = std::abs(cos(currentAngle)) * hw +
               std::abs(sin(currentAngle)) * hh;

    float ey = std::abs(sin(currentAngle)) * hw +
               std::abs(cos(currentAngle)) * hh;

    if (x - ex < 0 || x + ex > width) {
        speedX *= -1;
        x = std::clamp(x, ex, width - ex);
    }
    if (y - ey < 0 || y + ey > height) {
        speedY *= -1;
        y = std::clamp(y, ey, height - ey);
    }
}

/**
 * Draws filled version of the object with Name/ID in the center and vertices as circles. If it is currently
 * colliding with another object, the outline will be drawn in a different color.
 * @param font      Font name
 * @param fontSize  Font size
 * @param fontColor Font color
 * @param drawText  Flag to draw text
 */
void Rect::Draw(const Font &font, int fontSize, const Color &fontColor, bool drawText) {
    //Draw Filled Rectangle
    DrawRectanglePro(rect, {rect.width / 2.f, rect.height / 2.0f}, angle,
                     ColorFromNormalized({color[0], color[1], color[2], 1.0f}));
    //Draw Outline
    Vector2 p0 = {vertices[0].x, vertices[0].y};
    Vector2 p1 = {vertices[1].x, vertices[1].y};
    Vector2 p2 = {vertices[2].x, vertices[2].y};
    Vector2 p3 = {vertices[3].x, vertices[3].y};

    Color c = ColorFromNormalized({lineColor[0], lineColor[1], lineColor[2], 1.0f});
    DrawLineEx(p0, p1, 5, c);
    DrawLineEx(p1, p2, 5, c);
    DrawLineEx(p2, p3, 5, c);
    DrawLineEx(p3, p0, 5, c);

    //Draw Text
    if (drawText) {
        Vector2 textSize = MeasureTextEx(font, name.c_str(), fontSize, 1);
        DrawTextEx(font, name.c_str(), {x - textSize.x / 2, y - textSize.y / 2}, fontSize, 1, fontColor);
    }
    //Draw vertices
    for (Vec2 &v: vertices) {
        DrawCircleV({v.x, v.y}, 4, ColorFromNormalized({1.0f, 0.6f, 0.0f, 1.0f}));
    }
}
