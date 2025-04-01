#pragma once

// NOTE :
// This script may not me used because we have the glm library who does the job better

struct Vector2
{
    float X;
    float Y;

    Vector2() = default;
    Vector2(const float p_x, const float p_y) : X(p_x), Y(p_y) {}
};

struct Vector3
{
    float X;
    float Y;
    float Z;

    Vector3() = default;
    Vector3(const float p_x, const float p_y, const float p_z) : X(p_x), Y(p_y), Z(p_z) {}
};

struct Vector4
{
    float X;
    float Y;
    float Z;
    float W;

    Vector4() = default;
    Vector4(const float p_x, const float p_y, const float p_z, const float p_w) : X(p_x), Y(p_y), Z(p_z), W(p_w) {}
};
