#pragma once

#include <vcruntime_typeinfo.h>
#include <vector>
#include <GL/glew.h>

#include "../../../ExternalTools/MessageDebugger/MessageDebugger.h"

/// <summary>
/// Represents a single attribute in the vertex buffer layout, describing the type, count,
/// and whether the values should be normalized for OpenGL usage. </summary>
struct VertexBufferLayoutElements
{
    unsigned int Type;
    unsigned int Count;
    unsigned char IsValueNormalized;

    /// <summary>
    /// Returns the size (in bytes) of the given OpenGL type.
    /// <para> Throws a runtime error if the type is not planned. </para> </summary>
    static unsigned int GetTypeSize(unsigned int p_type)
    {
        switch (p_type)
        {
            case GL_FLOAT:          return 4;
            
            case GL_INT:            return 4;
            case GL_UNSIGNED_INT:   return 4;
            
            case GL_BYTE:           return 1;
            case GL_UNSIGNED_BYTE:  return 1;

            default:
                PRINT_ERROR_RUNTIME(true, std::string("The given type '") + std::to_string(p_type) + "' is not planned in the switch")
                return 0;
        }
    }
};

/// <summary>
/// Utility class that defines the layout of a Vertex Buffer Object (VBO) for OpenGL,
/// allowing the user to define the type, count, and normalization state of each vertex attribute.
///
/// <para> Only certain planned types (example : float, unsigned int, char) are supported through template specializations
/// of the <c> PushBack() </c> method. If you want you can implement more of them. </para>
///
/// <para> <b> Usage example: </b> </para>
/// <code>
/// VertexBufferLayoutObject layoutObject;
/// layoutObject.PushBack-float-(3, false);        // Position (x, y, z)
/// layoutObject.PushBack-float-(2, false);        // UV (u, v)
/// layoutObject.PushBack-unsigned int-(1, false); // Entity ID
/// </code>
///
/// <para> <b> Notes: </b> </para>
/// <list type="bullet">
/// <item> <description> The stride is updated automatically based on the types and count. </description> </item>
/// <item> <description> Use <c> GetLayoutElements() </c> and <c> GetStride() </c> to retrieve the internal layout and
/// total size (in bytes) of the vertex attributes. 
/// </description> </item> </list> </summary>
class VertexBufferLayoutObject
{

private:

    std::vector<VertexBufferLayoutElements> _layoutElements;
    unsigned int _stride;
    
public:

    VertexBufferLayoutObject();

    /// <summary>
    /// Gets the list of layout elements added to this buffer layout. </summary>
    inline const std::vector<VertexBufferLayoutElements>& GetLayoutElements() const& { return _layoutElements; }
    
    /// <summary>
    /// Gets the total stride (in bytes) of this layout, calculated from the sum of all added element sizes. </summary>
    inline unsigned int GetStride() const { return _stride; }


#pragma region // -=- PushBack methods -=- //

/// <summary>
/// Adds a new element to the layout with the given type T.
/// <para> This method is only valid for types that are explicitly specialized. </para>
/// <para> If an unsupported type is given, a runtime error is thrown and linking may fail. </para> </summary>
template <typename T>
void PushBack(const unsigned int p_count, const bool p_isValueNormalized)
{
    PRINT_ERROR_RUNTIME(true,
        std::string("The type you gave to the method 'PushBack' is not implemented. Please implement it or don't use it.\n - Type name : '") +  typeid(T).name() + "'"
    );
}

template <>
void PushBack<float>(const unsigned int p_count, const bool p_isValueNormalized)
{
    _layoutElements.push_back({ GL_FLOAT, p_count, p_isValueNormalized });

    _stride += p_count * VertexBufferLayoutElements::GetTypeSize(GL_FLOAT);
}

template <>
void PushBack<unsigned int>(const unsigned int p_count, const bool p_isValueNormalized)
{
    _layoutElements.push_back({ GL_UNSIGNED_INT, p_count, p_isValueNormalized });

    _stride += p_count * VertexBufferLayoutElements::GetTypeSize(GL_UNSIGNED_INT);
}

template <>
void PushBack<char>(const unsigned int p_count, const bool p_isValueNormalized)
{
    _layoutElements.push_back({ GL_BYTE, p_count, p_isValueNormalized });

    _stride += p_count * VertexBufferLayoutElements::GetTypeSize(GL_BYTE);
}

#pragma endregion
    
};