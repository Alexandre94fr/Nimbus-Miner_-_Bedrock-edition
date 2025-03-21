#pragma once

#include "VertexBufferObject.h"
#include "VertexBufferLayoutObject.h"

/// <summary>
/// A VAO (Vertex Array Object)
///
/// <para> Contains the configuration for reading data from VBOs </para>
/// 
/// <para> It does not contain the data itself, but records : </para>
/// <para> - which VBOs are used </para>
/// <para> - which glVertexAttribPointer are active </para>
/// <para> - which glEnableVertexAttribArrays are activated </para>
/// <para> - the Index Buffer(GL_ELEMENT_ARRAY_BUFFER) too </para>
///
/// <code>
/// +--------------------------------+
/// |    VAO | Vertex Array Object   |
/// +--------------------------------+
/// |  
/// | +------------------------------+
/// | |  VBO | Vertex Buffer Object  |
/// | |                              |
/// | |  (GL_ARRAY_BUFFER)           |
/// | |  Contains the vertices       |
/// | |  raw data                    |
/// | +------------------------------+
/// |
/// | +------------------------------+
/// | |  IBO | Index Buffer Object   |
/// | |                              |
/// | |  (GL_ELEMENT_ARRAY_BUFFER)   |
/// | |  Contains indexes            |
/// | |  that say which vertex       |
/// | |  to draw in the given order  |
/// | +------------------------------+
/// |
/// +--------------------------------+
/// </code> </summary>
class VertexArrayObject
{

private:
    
    unsigned int _vertexArrayObjectID;
    
public:

    VertexArrayObject();
    ~VertexArrayObject();
    
    void AddBuffer(const VertexBufferObject& p_vertexBufferObject, const VertexBufferLayoutObject& p_vertexLayoutObject) const;
    
    void Bind() const;
    void Unbind() const;
};