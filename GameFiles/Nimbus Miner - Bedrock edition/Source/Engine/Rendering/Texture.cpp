#include "Texture.h"

#include "DebuggingConstants.h"
#include "GL/glew.h"
#include "stb_image.h"

#include "MessageDebugger/MessageDebugger.h"


Texture::Texture(const std::string& p_filePath)
{
    // Setting local variables
    _inGpuId = 0;
    
    _filePath = p_filePath;

    _width = 0;
    _height = 0;
    _bitsPerPixel = 0;
    
    _localDataBuffer = nullptr;

    // Flipping the start position (telling OpenGL that we want to start at the top left of the image [PNG type file])
    stbi_set_flip_vertically_on_load(1);
    _localDataBuffer = stbi_load(_filePath.c_str(), &_width, &_height, &_bitsPerPixel, 4);

    if (_localDataBuffer == nullptr) 
        PRINT_ERROR_RUNTIME(true, "Failed to load texture : " + _filePath)

    if (IS_TEXTURE_LOADING_DEBUGGING_ON)
    {
        PRINT_MESSAGE_RUNTIME(std::string("Texture loaded successfully.") +
            "\n - File path : " + _filePath + 
            "\n - Width     : " + std::to_string(_width) + 
            "\n - Height    : " + std::to_string(_height) + 
            "\n - BPP       : " + std::to_string(_bitsPerPixel)
        )
    }
    
    glGenTextures(1, &_inGpuId);
    glBindTexture(GL_TEXTURE_2D, _inGpuId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // S = X coordinates
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // y = Y coordinates

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA8, // The format OpenGL use to store the data
        _width,
        _height,
        0, // 
        GL_RGBA, // The format of the data we sent out to OpenGL
        GL_UNSIGNED_BYTE, // The type of one data
        _localDataBuffer // The size of our data (use to allocate space)
    );
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    stbi_image_free(_localDataBuffer);
}

Texture::~Texture()
{
    // NOTE : We already delete the _localDataBuffer in the constructor so no need to delete it once more

    // Deleting texture from the GPU
    glDeleteTextures(1, &_inGpuId);
}

void Texture::Bind(const unsigned int p_textureSlot) const
{
    if (p_textureSlot > 31)
    {
        PRINT_ERROR_RUNTIME(true, "The given p_textureSlot is superior to 31, OpenGL only let us use 31 texture slots.");
        return;
    }

    glActiveTexture(GL_TEXTURE0 + p_textureSlot);
    glBindTexture(GL_TEXTURE_2D, _inGpuId);
}

void Texture::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}