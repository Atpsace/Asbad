#pragma once
#include "../core/types.h"
#include <cstdint>

namespace Renderer {

// Color utilities
struct Color {
    uint8_t r, g, b, a;
    
    static Color FromFloat(float r, float g, float b, float a = 1.0f) {
        return {(uint8_t)(r * 255), (uint8_t)(g * 255), (uint8_t)(b * 255), (uint8_t)(a * 255)};
    }
    
    uint32_t ToU32() const {
        return ((uint32_t)a << 24) | ((uint32_t)b << 16) | ((uint32_t)g << 8) | r;
    }
};

// Forward declare ImDrawList
struct ImDrawList;

class Draw {
public:
    // Box rendering
    static void Box2D(ImDrawList* draw, Vec2 tl, Vec2 br, uint32_t color, float thickness = 1.5f);
    static void Box3D(ImDrawList* draw, const Vec3* corners, const Matrix4x4& vm, int screenW, int screenH, uint32_t color, float thickness = 1.5f);
    static void BoxCorners(ImDrawList* draw, Vec2 tl, Vec2 br, float armLength, uint32_t color, float thickness = 1.5f);
    
    // Bar rendering
    static void HealthBar(ImDrawList* draw, Vec2 pos, float width, float height, float percent, bool gradient = true);
    static void ArmorBar(ImDrawList* draw, Vec2 pos, float width, float height, float percent);
    static void AmmoBar(ImDrawList* draw, Vec2 pos, float width, float height, float percent);
    
    // Skeleton rendering
    struct BoneLink { uint8_t a, b; };
    static void Skeleton(ImDrawList* draw, const Vec3* bones, const BoneLink* links, size_t linkCount, 
                        const Matrix4x4& vm, int screenW, int screenH, uint32_t color, float thickness = 1.5f);
    
    // Text rendering
    static void Text(ImDrawList* draw, Vec2 pos, const char* text, uint32_t color, float fontSize = 13.0f, bool shadow = true);
    static void TextCentered(ImDrawList* draw, Vec2 center, const char* text, uint32_t color, float fontSize = 13.0f, bool shadow = true);
    
    // Primitives
    static void Line(ImDrawList* draw, Vec2 start, Vec2 end, uint32_t color, float thickness = 1.0f);
    static void Circle(ImDrawList* draw, Vec2 center, float radius, uint32_t color, bool filled = false, float thickness = 1.0f);
    static void Rect(ImDrawList* draw, Vec2 tl, Vec2 br, uint32_t color, bool filled = false, float rounding = 0.0f, float thickness = 1.0f);
    static void Triangle(ImDrawList* draw, Vec2 p1, Vec2 p2, Vec2 p3, uint32_t color, bool filled = false, float thickness = 1.0f);
};

} // namespace Renderer
