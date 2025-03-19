#pragma once

/// <summary>
/// Creating a VBO (Vertex Buffer Object).
/// <para> Contains the raw data (vertex positions, colors, normals, UVs, etc...) we want to send to the GPU. </para> </summary>
class VertexBufferObject
{
    
private:

    // The program ID of what program will render things on the screen (in our case it's OpenGL)
    unsigned int _renderingProgramID;

public:

    /// <summary>
    /// Will created a VBO (Vertex Buffer Object).
    ///
    /// <para> Contains the raw data (vertex positions, colors, normals, UVs, etc...) you want to send to the GPU. </para> </summary>
    /// <param name = "p_data"> The data you want to send to the GPU (vertex positions, colors, normals, UVs, etc...) </param>
    /// <param name = "p_bytesSize"> The size (in bytes) of all the data you gave </param>
    VertexBufferObject(const void* p_data, unsigned int p_bytesSize);
    ~VertexBufferObject();

    void Bind() const;
    void Unbind() const;

    // TODO : SetData method
    
};