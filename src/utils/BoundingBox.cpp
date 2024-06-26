#include "BoundingBox.h"


void BoundingBox::Initialize(ImVec2 point, float rectangleSize, bool useHalfSize) {
    float sizeMultiplier = useHalfSize ? 0.5f : 1.0f;
    float halfSize = rectangleSize * sizeMultiplier;
    float x1 = point.x - halfSize;
    float y1 = point.y - halfSize;
    float x2 = point.x + halfSize;
    float y2 = point.y + halfSize;
    Initialize(y1, x1, y2, x2);
}

void BoundingBox::Initialize(float top, float left, float bottom, float right) {
    this->top = top;
    this->left = left;
    this->bottom = bottom;
    this->right = right;
    area = (right - left) * (bottom - top);
}


BoundingBox::BoundingBox(ImVec2 point, float rectangleSize) {
    Initialize(point, rectangleSize, false);
}

BoundingBox::BoundingBox(ImVec2 point, float rectangleSize, bool useHalfSize) {
    Initialize(point, rectangleSize, useHalfSize);
}


BoundingBox::BoundingBox(ImVec2 point) {
    Initialize(point.y, point.x, point.y, point.x);
}

BoundingBox::BoundingBox(float top, float left, float bottom, float right) {
    Initialize(top, left, bottom, right);
}

BoundingBox::BoundingBox(ImVec2 topLeft, ImVec2 bottomRight) {
    Initialize(topLeft.y, topLeft.x, bottomRight.y, bottomRight.x);
}

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
    return area;
}

float BoundingBox::GetSizeX() const {
    return right - left;
}

float BoundingBox::GetSizeY() const {
    return bottom - top;
}

float BoundingBox::GetLeft() const {
    return left;
}

float BoundingBox::GetRight() const {
    return right;
}

float BoundingBox::GetTop() const {
    return top;
}

float BoundingBox::GetBottom() const {
    return bottom;
}

bool BoundingBox::Overlaps(const BoundingBox& other) const {
    return (right >= other.left && left <= other.right && bottom >= other.top && top <= other.bottom);
}

bool BoundingBox::OverlapsVector(const ImVec2& other) const {
    return (other.x >= left && other.x <= right && other.y >= top && other.y <= bottom);
}


/// <summary>
/// How much other overlaps with current
/// </summary>
/// <param name="other"></param>
/// <returns></returns>
float BoundingBox::OverlapDegree(const BoundingBox& other) const {
    float overlapLeft = std::max(left, other.left);
    float overlapRight = std::min(right, other.right);
    float overlapTop = std::max(top, other.top);
    float overlapBottom = std::min(bottom, other.bottom);

    float overlapWidth = std::max(0.0f, overlapRight - overlapLeft);
    float overlapHeight = std::max(0.0f, overlapBottom - overlapTop);

    float overlapArea = overlapWidth * overlapHeight;
    float boxArea = (right - left) * (bottom - top);

    if (boxArea == 0.0f) {
        // Avoid division by zero
        return 0.0f;
    }

    float overlapPercentage = overlapArea / boxArea;

    // Ensure order of operation
    if (overlapPercentage > 1.0f) {
        return 1.0f;
    }
    else {
        return overlapPercentage;
    }
}

// Creates new BoundingBox for current and other bounding boxes
BoundingBox BoundingBox::Merge(const BoundingBox& other) const {    
    return *this + other;
}


BoundingBox& BoundingBox::operator+=(const BoundingBox& rhs) {
    this->left = std::min(this->left, rhs.left);
    this->top = std::min(this->top, rhs.top);
    this->right = std::max(this->right, rhs.right);
    this->bottom = std::max(this->bottom, rhs.bottom);
    return *this;
}

// BoundingBox + BoundingBox
BoundingBox operator+(BoundingBox lhs, const BoundingBox& rhs) {
    lhs += rhs;
    return lhs;
}


// Might break
BoundingBox operator+(const ImVec2& lhs, BoundingBox rhs) {
    return rhs + lhs;
}

BoundingBox operator+(BoundingBox lhs, const ImVec2& rhs) {
    lhs.left = std::min(lhs.left, rhs.x);
    lhs.top = std::min(lhs.top, rhs.y);
    lhs.right = std::max(lhs.right, rhs.x);
    lhs.bottom = std::max(lhs.bottom, rhs.y);

    return lhs;
}

bool boundingbox_overlaps(const BoundingBox& box1, const BoundingBox& box2) {
    return box1.Overlaps(box2);
}

double boundingbox_overlap_degree(const BoundingBox& box1, const BoundingBox& box2) {
    return box1.OverlapDegree(box2);
}

BoundingBox boundingbox_merge(const BoundingBox& box1, const BoundingBox& box2) {
    return box1.Merge(box2);
}

