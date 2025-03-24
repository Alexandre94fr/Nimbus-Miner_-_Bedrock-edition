#include "IndexBufferObject.h"

#include <GL/glew.h>

#include "MessageDebugger/MessageDebugger.h"

IndexBufferObject::IndexBufferObject(const unsigned int* p_data, unsigned int p_dataCount)
{
    _indexesCount = p_dataCount;
    
    glGenBuffers(1, &_renderingProgramID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _renderingProgramID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, p_dataCount * sizeof(unsigned int), p_data, GL_STATIC_DRAW);
}

IndexBufferObject::~IndexBufferObject()
{
    glDeleteBuffers(1, &_renderingProgramID);
}

void IndexBufferObject::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _renderingProgramID);
}

void IndexBufferObject::Unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}