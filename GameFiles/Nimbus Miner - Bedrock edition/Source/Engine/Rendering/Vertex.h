#pragma once

#include <vector>

#include "GLM/vec2.hpp"
#include "GLM/vec3.hpp"

struct Vertex
{
    glm::vec3 Position;
    glm::vec2 TextureUV;
    
    // Returns the number of float insind the Vertex struct.
    // If you add a new variable insind the Vertex struct, please add the variable into the sum below.
    static constexpr int FLOATS_IN_VERTEX = 
        (sizeof(Position) + sizeof(TextureUV)) / sizeof(float);

    Vertex() = default;
    Vertex(const glm::vec3 p_position, const glm::vec2 p_textureUV) : Position(p_position), TextureUV(p_textureUV) {}

    std::vector<float> ToFloats() const
    {
        return
        {
            Position.x, Position.y, Position.z,
            TextureUV.x, TextureUV.y
        };
    }
};

std::vector<float> ConvertVerticesToFloatArray(const std::vector<Vertex>& p_vertices);