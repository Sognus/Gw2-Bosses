#include "BoundingBox.h"

BoundingBox::BoundingBox(float top, float left, float bottom, float right)
    : top(top), left(left), bottom(bottom), right(right) {}

ImVec2 BoundingBox::GetTopLeft() const {
    return ImVec2(left, top);
}

ImVec2 BoundingBox::GetTopRight() const {
    return ImVec2(right, top);
}

ImVec2 BoundingBox::GetBottomLeft() const {
    return ImVec2(left, bottom);
}

ImVec2 BoundingBox::GetBottomRight() const {
    return ImVec2(right, bottom);
}

ImVec2 BoundingBox::GetCenter() const {
    return ImVec2((left + right) * 0.5f, (top + bottom) * 0.5f);
}

float BoundingBox::GetArea() const {
    return (right - left) * (bottom - top);
}

float BoundingBox::GetSizeX() const {
    return right - left;
}

float BoundingBox::GetSizeY() const {
    return bottom - top;
}