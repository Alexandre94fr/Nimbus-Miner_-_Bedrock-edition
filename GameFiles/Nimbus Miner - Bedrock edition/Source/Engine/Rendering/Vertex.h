#pragma once

#include <vector>

#include "Vector.h"
#include <iostream>

struct Vertex
{
    Vector3 Position;
    Vector4 Color;
    Vector2 TextureUV;
    
    // Returns the number of float inside the Vertex struct.
    // If you add a new variable inside the Vertex struct, please add the variable into the sum below.
    // Plus, don't forget to add the return value of your new variable inside the ToFloats function.
    static constexpr int FLOATS_IN_VERTEX = 
        (sizeof(Position) + sizeof(Color) + sizeof(TextureUV)) / sizeof(float);

    Vertex() = default;
    Vertex(const Vector3& p_position, const Vector4& p_color, const Vector2& p_textureUV) :
        Position(p_position), Color(p_color), TextureUV(p_textureUV) {}

    std::vector<float> ToFloats() const
    {
        return
        {
            Position.X, Position.Y, Position.Z,
            Color.X, Color.Y, Color.Z, Color.W,
            TextureUV.X, TextureUV.Y,

            // Add more if necessary
            // BE SURE TO RESPECT THE ORDER OF VARIABLES
        };
    }
};

std::vector<float> ConvertVerticesToFloatArray(const std::vector<Vertex>& p_vertices);