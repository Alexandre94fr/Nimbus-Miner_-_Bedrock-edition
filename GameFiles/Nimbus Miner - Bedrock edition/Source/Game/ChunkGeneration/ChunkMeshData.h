#pragma once

#include <vector>

#include "Vertex.h"
#include "IndexBufferObject.h"

struct ChunkMeshData
{
    std::vector<Vertex> Vertices;

    /// <summary> The vertices draw order. </summary>
    std::vector<unsigned int> VerticesIndices;

    void Clear()
    {
        Vertices.clear();
        VerticesIndices.clear();
    }

    /// <summary>
    /// Returns a unsigned int array, you can also take the size of it with the 'p_outUintVectorCount' parameter.
    ///
    /// <para> <b> BEWARE ! Using this method will create the array dynamically,
    /// that means you will have to delete the array when you will have finished to use it. (Use the 'delete[]' keyword) </b> </para>
    /// </summary>
    static unsigned int* ConvertVectorToArray(const std::vector<unsigned int>& p_uintVector, unsigned int& p_outUintVectorCount)
    {
        p_outUintVectorCount = p_uintVector.size();
        
        unsigned int* array = new unsigned int[p_outUintVectorCount];
        
        for (size_t i = 0; i < p_outUintVectorCount; ++i) {
            array[i] = p_uintVector[i];
        }
        
        return array;
    }
};