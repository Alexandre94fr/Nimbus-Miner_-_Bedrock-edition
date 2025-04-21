#include "VertexArrayObject.h"

#include <iostream>
#include <ostream>
#include <GL/glew.h>

VertexArrayObject::VertexArrayObject()
{
    glGenVertexArrays(1, &_vertexArrayObjectID);
}

VertexArrayObject::~VertexArrayObject()
{
    glDeleteVertexArrays(1, &_vertexArrayObjectID);
}

void VertexArrayObject::AddBuffer(const VertexBufferObject& p_vertexBufferObject, const VertexBufferLayoutObject& p_vertexLayoutObject) const
{
    // Bind ourselves
    Bind();

    // Bind the vertex buffer object we will use
    p_vertexBufferObject.Bind();

    const std::vector<VertexBufferLayoutElements>& vertexBufferLayoutElements = p_vertexLayoutObject.GetLayoutElements();
    unsigned int offset = 0;

    for (unsigned int i = 0; i < vertexBufferLayoutElements.size(); i++)
    {
        const VertexBufferLayoutElements& vertexBufferLayoutElement = vertexBufferLayoutElements[i];
        
        glVertexAttribPointer(
            i,                                              // - The index of the attribute (example : if you want to bind the second attribute of your vertex you will pass 1)
            vertexBufferLayoutElement.Count,                // - The number of data in your attribute (example : If you want three float in your attributes, you pass 3)
            vertexBufferLayoutElement.Type,                 // - The type of each of your data
            vertexBufferLayoutElement.IsValueNormalized,    // - Is the value of each data will be normalized by OpenGL
            p_vertexLayoutObject.GetStride(),               // - The size (in bytes) of all attributes of this vertex
                                                            // (example : if you have a vertex that have 3 attributes and each of them weight 12 bytes,
                                                            //            that's mean you pass 12  * 3 (36 bytes))
            (const void*)offset                             // - Offset (in bytes) from the beginning of the vertex to this attribute
                                                            // (example : if this attribute is the first one in the vertex, use (void*)0;
                                                            //            if it's the second and the first is a vec3 (3 floats), use (void*)(3 * sizeof(float)))
        );
        glEnableVertexAttribArray(i);

        /* More documentation
        
        // The 'glEnableVertexAttribArray' function will also link* the VertexArrayObject and the VertexBufferObject
        // that means we will only need to use one line of code to bind the two ( glBindVertexArray(vertexArrayObjectID); )

        // *The link between VBO and VAO is not direct or permanent,
        // it's just that when you call glVertexAttribPointer, OpenGL notes in the active VAO :
        // "For attribute 0, I need to search in such-and-such a VBO, with such-and-such a format." */
        
        offset += vertexBufferLayoutElement.Count * VertexBufferLayoutElements::GetTypeSize(vertexBufferLayoutElement.Type);
    }
}

void VertexArrayObject::Bind() const
{
    glBindVertexArray(_vertexArrayObjectID);
}

void VertexArrayObject::Unbind() const
{
    glBindVertexArray(0);
}