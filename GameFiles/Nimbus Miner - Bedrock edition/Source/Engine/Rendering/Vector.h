#pragma once

#include <stdexcept>

#pragma region - Float vectors -

struct Vector2
{
    float X;
    float Y;

    Vector2() = default;
    Vector2(const float p_x, const float p_y) : X(p_x), Y(p_y) {}

    static Vector2 Zero()     { return { 0.0f,  0.0f }; }
    static Vector2 One()      { return { 1.0f,  1.0f }; }

    static Vector2 Up()       { return { 0.0f,  1.0f }; }
    static Vector2 Down()     { return { 0.0f, -1.0f }; }
    static Vector2 Left()     { return {-1.0f,  0.0f }; }
    static Vector2 Right()    { return { 1.0f,  0.0f }; }

    #pragma region Operator[]
    
    float& operator[](const size_t p_index)
    {
        switch (p_index)
        {
            case 0: return X;
            case 1: return Y;
            default: throw std::out_of_range("Vector2 index out of range");
        }
    }

    const float& operator[](const size_t p_index) const
    {
        switch (p_index)
        {
            case 0: return X;
            case 1: return Y;
            default: throw std::out_of_range("Vector2 index out of range");
        }
    }
    #pragma endregion
};

struct Vector3
{
    float X;
    float Y;
    float Z;

    Vector3() = default;
    Vector3(const float p_x, const float p_y, const float p_z) : X(p_x), Y(p_y), Z(p_z) {}

    static Vector3 Zero()     { return { 0.0f,  0.0f,  0.0f }; }
    static Vector3 One()      { return { 1.0f,  1.0f,  1.0f }; }

    static Vector3 Up()       { return { 0.0f,  1.0f,  0.0f }; }
    static Vector3 Down()     { return { 0.0f, -1.0f,  0.0f }; }
    static Vector3 Left()     { return {-1.0f,  0.0f,  0.0f }; }
    static Vector3 Right()    { return { 1.0f,  0.0f,  0.0f }; }
    static Vector3 Forward()  { return { 0.0f,  0.0f,  1.0f }; }
    static Vector3 Backward() { return { 0.0f,  0.0f, -1.0f }; }

    #pragma region Operator[]
    
    float& operator[](const size_t p_index)
    {
        switch (p_index)
        {
            case 0: return X;
            case 1: return Y;
            case 2: return Z;
            default: throw std::out_of_range("Vector3 index out of range");
        }
    }

    const float& operator[](const size_t p_index) const
    {
        switch (p_index)
        {
            case 0: return X;
            case 1: return Y;
            case 2: return Z;
            default: throw std::out_of_range("Vector3 index out of range");
        }
    }
    #pragma endregion
};

struct Vector4
{
    float X;
    float Y;
    float Z;
    float W;

    Vector4() = default;
    Vector4(const float p_x, const float p_y, const float p_z, const float p_w) : X(p_x), Y(p_y), Z(p_z), W(p_w) {}

    static Vector4 Zero()     { return { 0.0f,  0.0f,  0.0f,  0.0f }; }
    static Vector4 One()      { return { 1.0f,  1.0f,  1.0f,  1.0f }; }

    static Vector4 Up()       { return { 0.0f,  1.0f,  0.0f,  0.0f }; }
    static Vector4 Down()     { return { 0.0f, -1.0f,  0.0f,  0.0f }; }
    static Vector4 Left()     { return {-1.0f,  0.0f,  0.0f,  0.0f }; }
    static Vector4 Right()    { return { 1.0f,  0.0f,  0.0f,  0.0f }; }
    static Vector4 Forward()  { return { 0.0f,  0.0f,  1.0f,  0.0f }; }
    static Vector4 Backward() { return { 0.0f,  0.0f, -1.0f,  0.0f }; }

    #pragma region Operator[]
    
    float& operator[](const size_t p_index)
    {
        switch (p_index)
        {
            case 0: return X;
            case 1: return Y;
            case 2: return Z;
            case 3: return W;
            default: throw std::out_of_range("Vector4 index out of range");
        }
    }

    const float& operator[](const size_t p_index) const
    {
        switch (p_index)
        {
            case 0: return X;
            case 1: return Y;
            case 2: return Z;
            case 3: return W;
            default: throw std::out_of_range("Vector4 index out of range");
        }
    }
    #pragma endregion
};

#pragma endregion

#pragma region - Int vectors -

struct Vector2Int
{
    int X;
    int Y;

    Vector2Int() = default;
    Vector2Int(const int p_x, const int p_y) : X(p_x), Y(p_y) {}

    static Vector2Int Zero()    { return { 0,  0 }; }
    static Vector2Int One()     { return { 1,  1 }; }

    static Vector2Int Up()      { return { 0,  1 }; }
    static Vector2Int Down()    { return { 0, -1 }; }
    static Vector2Int Left()    { return {-1,  0 }; }
    static Vector2Int Right()   { return { 1,  0 }; }

    #pragma region Operator[]
    
    int& operator[](const size_t p_index)
    {
        switch (p_index)
        {
            case 0: return X;
            case 1: return Y;
            default: throw std::out_of_range("Vector2Int index out of range");
        }
    }

    const int& operator[](const size_t p_index) const
    {
        switch (p_index)
        {
            case 0: return X;
            case 1: return Y;
            default: throw std::out_of_range("Vector2Int index out of range");
        }
    }
    #pragma endregion
};

struct Vector3Int
{
    int X;
    int Y;
    int Z;

    Vector3Int() = default;
    Vector3Int(const int p_x, const int p_y, const int p_z) : X(p_x), Y(p_y), Z(p_z) {}

    static Vector3Int Zero()    { return { 0,  0,  0 }; }
    static Vector3Int One()     { return { 1,  1,  1 }; }

    static Vector3Int Up()      { return { 0,  1,  0 }; }
    static Vector3Int Down()    { return { 0, -1,  0 }; }
    static Vector3Int Left()    { return {-1,  0,  0 }; }
    static Vector3Int Right()   { return { 1,  0,  0 }; }
    static Vector3Int Forward() { return { 0,  0,  1 }; }
    static Vector3Int Backward(){ return { 0,  0, -1 }; }

    #pragma region Operator[]
    
    int& operator[](const size_t p_index)
    {
        switch (p_index)
        {
            case 0: return X;
            case 1: return Y;
            case 2: return Z;
            default: throw std::out_of_range("Vector3Int index out of range");
        }
    }

    const int& operator[](const size_t p_index) const
    {
        switch (p_index)
        {
            case 0: return X;
            case 1: return Y;
            case 2: return Z;
            default: throw std::out_of_range("Vector3Int index out of range");
        }
    }
    #pragma endregion
};

struct Vector4Int
{
    int X;
    int Y;
    int Z;
    int W;

    Vector4Int() = default;
    Vector4Int(const int p_x, const int p_y, const int p_z, const int p_w) : X(p_x), Y(p_y), Z(p_z), W(p_w) {}

    static Vector4Int Zero()    { return { 0,  0,  0,  0 }; }
    static Vector4Int One()     { return { 1,  1,  1,  1 }; }

    static Vector4Int Up()      { return { 0,  1,  0,  0 }; }
    static Vector4Int Down()    { return { 0, -1,  0,  0 }; }
    static Vector4Int Left()    { return {-1,  0,  0,  0 }; }
    static Vector4Int Right()   { return { 1,  0,  0,  0 }; }
    static Vector4Int Forward() { return { 0,  0,  1,  0 }; }
    static Vector4Int Backward(){ return { 0,  0, -1,  0 }; }

    #pragma region Operator[]
    
    int& operator[](const size_t p_index)
    {
        switch (p_index)
        {
            case 0: return X;
            case 1: return Y;
            case 2: return Z;
            case 3: return W;
            default: throw std::out_of_range("Vector4Int index out of range");
        }
    }

    const int& operator[](const size_t p_index) const
    {
        switch (p_index)
        {
            case 0: return X;
            case 1: return Y;
            case 2: return Z;
            case 3: return W;
            default: throw std::out_of_range("Vector4Int index out of range");
        }
    }
    #pragma endregion
};

#pragma endregion

#pragma region - Unsigned int vectors -

/// <summary> <b> Please don't pass negative values, doing so will result on your values being modified (2^32 - yourValue) </b> </summary>
struct Vector2Uint
{
    unsigned int X;
    unsigned int Y;

    Vector2Uint() = default;
    Vector2Uint(const unsigned int p_x, const unsigned int p_y) : X(p_x), Y(p_y) {}

    static Vector2Uint Zero()   { return { 0,  0 }; }
    static Vector2Uint One()    { return { 1,  1 }; }

    #pragma region Operator[]
    
    unsigned int& operator[](const size_t p_index)
    {
        switch (p_index)
        {
            case 0: return X;
            case 1: return Y;
            default: throw std::out_of_range("Vector2Uint index out of range");
        }
    }

    const unsigned int& operator[](const size_t p_index) const
    {
        switch (p_index)
        {
            case 0: return X;
            case 1: return Y;
            default: throw std::out_of_range("Vector2Uint index out of range");
        }
    }
    #pragma endregion
};

/// <summary> <b> Please don't pass negative values, doing so will result on your values being modified (2^32 - yourValue) </b> </summary>
struct Vector3Uint
{
    unsigned int X;
    unsigned int Y;
    unsigned int Z;

    Vector3Uint() = default;
    Vector3Uint(const unsigned int p_x, const unsigned int p_y, const unsigned int p_z) : X(p_x), Y(p_y), Z(p_z) {}

    static Vector3Uint Zero()   { return { 0,  0,  0 }; }
    static Vector3Uint One()    { return { 1,  1,  1 }; }

    #pragma region Operator+

    Vector3Uint operator+(const Vector3Uint& p_vector) const
    {
        return {
            X + p_vector.X,
            Y + p_vector.Y,
            Z + p_vector.Z
        };
    }
    #pragma endregion 
    
    #pragma region Operator[]
    
    unsigned int& operator[](const size_t p_index)
    {
        switch (p_index)
        {
            case 0: return X;
            case 1: return Y;
            case 2: return Z;
            default: throw std::out_of_range("Vector3Uint index out of range");
        }
    }

    const unsigned int& operator[](const size_t p_index) const
    {
        switch (p_index)
        {
            case 0: return X;
            case 1: return Y;
            case 2: return Z;
            default: throw std::out_of_range("Vector3Uint index out of range");
        }
    }
    #pragma endregion
};

/// <summary> <b> Please don't pass negative values, doing so will result on your values being modified (2^32 - yourValue) </b> </summary>
struct Vector4Uint
{
    unsigned int X;
    unsigned int Y;
    unsigned int Z;
    unsigned int W;

    Vector4Uint() = default;
    Vector4Uint(const unsigned int p_x, const unsigned int p_y, const unsigned int p_z, const unsigned int p_w) : X(p_x), Y(p_y), Z(p_z), W(p_w) {}

    static Vector4Uint Zero()   { return { 0,  0,  0,  0 }; }
    static Vector4Uint One()    { return { 1,  1,  1,  1 }; }

    #pragma region Operator[]
    
    unsigned int& operator[](const size_t p_index)
    {
        switch (p_index)
        {
            case 0: return X;
            case 1: return Y;
            case 2: return Z;
            case 3: return W;
            default: throw std::out_of_range("Vector4Uint index out of range");
        }
    }

    const unsigned int& operator[](const size_t p_index) const
    {
        switch (p_index)
        {
            case 0: return X;
            case 1: return Y;
            case 2: return Z;
            case 3: return W;
            default: throw std::out_of_range("Vector4Uint index out of range");
        }
    }
    #pragma endregion
};

#pragma endregion