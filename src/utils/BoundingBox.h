#include "../Globals.h"

#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

/// <summary>
/// Object that represents 2D area for rendering purposes  
/// </summary>
class BoundingBox {
public:
    /// <summary>
    /// Constructor from individual coordinate values
    /// </summary>
    /// <param name="t">Top y coordinate</param>
    /// <param name="l">Left x coordinate</param>
    /// <param name="b">Bottom y coordinate</param>
    /// <param name="r">Right x coordinate</param>
    BoundingBox(float t, float l, float b, float r);

    /// <summary>
    /// Constructor for empty area at given location
    /// </summary>
    /// <param name="point">Initial location</param>
    BoundingBox(ImVec2 point);

    /// <summary>
    /// Constructor for area from Imgui's points
    /// </summary>
    /// <param name="topLeft">Top-left x, y coordinates</param>
    /// <param name="bottomRight">Bottom-right x, y coordinates</param>
    BoundingBox(ImVec2 topLeft, ImVec2 bottomRight);
    
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

    bool Overlaps(const BoundingBox& other) const;
    float OverlapDegree(const BoundingBox& other) const;

    BoundingBox Merge(const BoundingBox& other) const;
    BoundingBox& operator+=(const BoundingBox& rhs);
    friend BoundingBox operator+(BoundingBox lhs, const BoundingBox& rhs);
    friend BoundingBox operator+(BoundingBox lhs, const ImVec2& rhs);
    friend BoundingBox operator+(const ImVec2& lhs, BoundingBox rhs);

private:
    float top;
    float left;
    float bottom;
    float right;
    float area;

    void Initialize(float top, float left, float bottom, float right);
};

#endif //BOUNDING_BOX_H