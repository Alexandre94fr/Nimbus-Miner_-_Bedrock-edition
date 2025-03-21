#pragma once
#include "IndexBufferObject.h"
#include "Shader.h"
#include "VertexArrayObject.h"

class Renderer
{
    
public:

    void Clear() const;
    void Draw(const VertexArrayObject& p_vertexArrayObject, const IndexBufferObject& p_indexBufferObject, const Shader& p_shader) const;
    
};
