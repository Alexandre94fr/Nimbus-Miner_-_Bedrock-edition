#pragma once

#pragma region - Float vectors -

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

#pragma endregion

#pragma region - Int vectors -

struct Vector2Int
{
    int X;
    int Y;

    Vector2Int() = default;
    Vector2Int(const int p_x, const int p_y) : X(p_x), Y(p_y) {}
};

struct Vector3Int
{
    int X;
    int Y;
    int Z;

    Vector3Int() = default;
    Vector3Int(const int p_x, const int p_y, const int p_z) : X(p_x), Y(p_y), Z(p_z) {}
};

struct Vector4Int
{
    int X;
    int Y;
    int Z;
    int W;

    Vector4Int() = default;
    Vector4Int(const int p_x, const int p_y, const int p_z, const int p_w) : X(p_x), Y(p_y), Z(p_z), W(p_w) {}
};

#pragma endregion

#pragma region - Unsigned int vectors -

struct Vector2Uint
{
    unsigned int X;
    unsigned int Y;

    Vector2Uint() = default;
    Vector2Uint(const unsigned int p_x, const unsigned int p_y) : X(p_x), Y(p_y) {}
};

struct Vector3Uint
{
    unsigned int X;
    unsigned int Y;
    unsigned int Z;

    Vector3Uint() = default;
    Vector3Uint(const unsigned int p_x, const unsigned int p_y, const unsigned int p_z) : X(p_x), Y(p_y), Z(p_z) {}
};

struct Vector4Uint
{
    unsigned int X;
    unsigned int Y;
    unsigned int Z;
    unsigned int W;

    Vector4Uint() = default;
    Vector4Uint(const unsigned int p_x, const unsigned int p_y, const unsigned int p_z, const unsigned int p_w) : X(p_x), Y(p_y), Z(p_z), W(p_w) {}
};

#pragma endregion