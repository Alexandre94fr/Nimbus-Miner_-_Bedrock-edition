#pragma once

/// <summary> Contains indexes that say "which vertex to draw at what time". </summary>
class IndexBufferObject
{

private:

    // The program ID of what program will render things on the screen (in our case it's OpenGL)
    unsigned int _renderingProgramID;

    unsigned int _indexesCount;

public:

    /// <summary>
    /// Will created a IBO (Index Buffer Object).
    ///
    /// <para> Contains indexes that say which vertex to draw in the given order. </para> </summary>
    /// <param name = "p_data"> A table of unsigned int </param>
    /// <param name = "p_dataCount"> The number of indexes you gave </param>
    IndexBufferObject(const unsigned int* p_data, unsigned int p_dataCount);
    ~IndexBufferObject();

    void Bind() const;
    void Unbind() const;

    inline unsigned int GetRenderingProgramID() const { return _renderingProgramID; }
    inline unsigned int GetIndexesCount() const { return _indexesCount; }
    
    // TODO : SetData method
};