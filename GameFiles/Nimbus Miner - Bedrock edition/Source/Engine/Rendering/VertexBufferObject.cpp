#include "VertexBufferObject.h"

#include <GL/glew.h>

#include "Vertex.h"

VertexBufferObject::VertexBufferObject(const void* p_data, unsigned int p_bytesSize)
{
    glGenBuffers(1, &_vertexBufferObjectID);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObjectID);
    glBufferData(GL_ARRAY_BUFFER, p_bytesSize, p_data, GL_STATIC_DRAW);
}

VertexBufferObject::~VertexBufferObject()
{
    glDeleteBuffers(1, &_vertexBufferObjectID);
}

void VertexBufferObject::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObjectID);
}

void VertexBufferObject::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBufferObject::SetData(const void* p_data, unsigned int p_bytesSize) const
{
    Bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, p_bytesSize, p_data);
}