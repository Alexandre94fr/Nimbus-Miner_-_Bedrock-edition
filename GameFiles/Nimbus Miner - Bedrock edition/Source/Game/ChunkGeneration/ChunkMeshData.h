#pragma once

#include <vector>

#include "Vertex.h"
#include "IndexBufferObject.h"

struct ChunkMeshData
{
    std::vector<Vertex> Vertices;
    
    IndexBufferObject* IndexBufferObject;

    void Clear()
    {
        Vertices.clear();
        IndexBufferObject = nullptr;
    }
};