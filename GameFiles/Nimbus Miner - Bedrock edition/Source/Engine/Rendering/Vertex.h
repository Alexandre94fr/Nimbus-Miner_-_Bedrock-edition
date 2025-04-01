#pragma once

#include <vector>

#include "Vector.h"

struct Vertex
{
    Vector3 Position;
    Vector3 Normal;
    Vector4 Color;
    Vector2 TexturePosition; // Also called UV
    
    // Returns the number of float inside the Vertex struct.
    // If you add a new variable inside the Vertex struct, please add the variable into the sum below.
    // Plus, don't forget to add the return value of your new variable inside the ToFloats function.
    static constexpr int FLOATS_IN_VERTEX = 
        (sizeof(Position) + sizeof(Normal) + sizeof(Color) + sizeof(TexturePosition)) / sizeof(float);

    Vertex() = default;
    Vertex(const Vector3& p_position, const Vector3& p_normal, const Vector4& p_color, const Vector2& p_texturePosition) :
        Position(p_position), Normal(p_normal), Color(p_color), TexturePosition(p_texturePosition) {}

    std::vector<float> ToFloats() const
    {
        return
        {
            Position.X, Position.Y, Position.Z,
            Normal.X, Normal.Y, Normal.Z,
            Color.X, Color.Y, Color.Z, Color.W,
            TexturePosition.X, TexturePosition.Y,

            // Add more if necessary
            // BE SURE TO RESPECT THE ORDER OF VARIABLES
        };
    }
};

std::vector<float> ConvertVerticesToFloatArray(const std::vector<Vertex>& p_vertices);