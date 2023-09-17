#include "../Globals.h"

#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

class BoundingBox {
public:
    BoundingBox(float t, float l, float b, float r);

    ImVec2 GetTopLeft() const;
    ImVec2 GetTopRight() const;
    ImVec2 GetBottomLeft() const;
    ImVec2 GetBottomRight() const;
    
    ImVec2 GetCenter() const;
    
    float GetArea() const;
    float GetSizeX() const;
    float GetSizeY() const;

    float GetLeft() const;
    float GetRight() const;
    float GetTop() const;
    float GetBottom() const;

private:
    float top;
    float left;
    float bottom;
    float right;
};

#endif //BOUNDING_BOX_H