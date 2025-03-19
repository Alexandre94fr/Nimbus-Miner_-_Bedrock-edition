#include "VertexBufferObject.h"

#include <GL/glew.h>

VertexBufferObject::VertexBufferObject(const void* p_data, unsigned int p_bytesSize)
{
    glGenBuffers(1, &_renderingProgramID);
    glBindBuffer(GL_ARRAY_BUFFER, _renderingProgramID);
    glBufferData(GL_ARRAY_BUFFER, p_bytesSize, p_data, GL_STATIC_DRAW);
}

VertexBufferObject::~VertexBufferObject()
{
    glDeleteBuffers(1, &_renderingProgramID);
}

void VertexBufferObject::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, _renderingProgramID);
}

void VertexBufferObject::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}