#pragma once
#include <cstdint>
#include <cmath>

struct Vec2 {
    float x = 0.0f, y = 0.0f;
    
    Vec2() = default;
    Vec2(float _x, float _y) : x(_x), y(_y) {}
    
    float Length() const { return sqrtf(x * x + y * y); }
    float Distance(const Vec2& o) const { return sqrtf((x - o.x) * (x - o.x) + (y - o.y) * (y - o.y)); }
};

struct Vec3 {
    float x = 0.0f, y = 0.0f, z = 0.0f;
    
    Vec3() = default;
    Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    
    float Length() const { return sqrtf(x * x + y * y + z * z); }
    float Distance(const Vec3& o) const {
        float dx = x - o.x;
        float dy = y - o.y;
        float dz = z - o.z;
        return sqrtf(dx * dx + dy * dy + dz * dz);
    }
    
    Vec3 operator-(const Vec3& o) const { return Vec3(x - o.x, y - o.y, z - o.z); }
    Vec3 operator+(const Vec3& o) const { return Vec3(x + o.x, y + o.y, z + o.z); }
    Vec3 operator*(float s) const { return Vec3(x * s, y * s, z * s); }
};

struct Matrix4x4 {
    float m[4][4];
};

struct BoneJointData {
    Vec3 pos;
};

enum class Team : uint8_t {
    None = 0,
    Spectator = 1,
    T = 2,
    CT = 3
};

enum class HitBox : int {
    Head = 0,
    Neck = 1,
    Chest = 2,
    Pelvis = 3
};
