#include "Renderer.h"

#include <iostream>

#include "ProjectConstants.h"

void Renderer::Clear()
{
    glClearColor(BACKGROUND_COLOR.x, BACKGROUND_COLOR.y, BACKGROUND_COLOR.z, BACKGROUND_COLOR.w);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw(const VertexArrayObject& p_vertexArrayObject, const IndexBufferObject& p_indexBufferObject, const Shader& p_shader)
{
    // Binding into the GPU the given data
    p_vertexArrayObject.Bind();
    p_indexBufferObject.Bind();
    p_shader.Bind();

    glDrawElements(GL_TRIANGLES, p_indexBufferObject.GetIndexesCount(), GL_UNSIGNED_INT, nullptr);
    // We use nullptr because we already bind the indexBufferObjectID before

    // NOTE :
    // If you want you can Unbind() the given data for debugging reason.
    // But always doing it, is a waste of resources because the next Draw will Bind() again (so the last data will be overridden)
}