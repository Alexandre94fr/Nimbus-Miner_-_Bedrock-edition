#include "Vertex.h"

std::vector<float> ConvertVerticesToFloatArray(const std::vector<Vertex>& p_vertices)
{
    std::vector<float> floatArrayResult;

    // Giving the correct size to the float array
    floatArrayResult.reserve(p_vertices.size() * Vertex::FLOATS_IN_VERTEX);

    for (int i = 0; i < p_vertices.size(); i++)
    {
        // Converting the Vertice struct into a vector<float> 
        std::vector<float> floatArray = p_vertices[i].ToFloats();

        // Insert all floats from floatArray into the end of floatArrayResult
        floatArrayResult.insert(floatArrayResult.end(), floatArray.begin(), floatArray.end());
    }

    return floatArrayResult;
}
