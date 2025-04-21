#include "Texture2DArray.h"

#include <iostream>

#include "DebuggingConstants.h"
#include "stb_image.h"
#include "GL/glew.h"

#include "MessageDebugger/MessageDebugger.h"

Texture2DArray::Texture2DArray(const std::vector<std::string>& p_filePaths)
{
    _inGpuId = 0; // Will be re-set in the code below
    _layerCount = static_cast<int>(p_filePaths.size());
    _width = 0; // Will be re-set in the code below
    _height = 0; // Will be re-set in the code below
    
    std::vector<unsigned char*> layerData(_layerCount);

    // Loading all images
    for (int i = 0; i < _layerCount; ++i)
    {
        int bitsPerPixel = 0;
        
        // Flipping the start position (telling OpenGL that we want to start at the top left of the image [PNG type file])
        stbi_set_flip_vertically_on_load(1);
        
        // Loading the sprite and setting the '_width', '_height', and '_bitsPerPixel' variables
        layerData[i] = stbi_load(p_filePaths[i].c_str(), &_width, &_height, &bitsPerPixel, 4);
        
        if (layerData[i] == nullptr)
        {
            PRINT_ERROR_RUNTIME(true, std::string("Failed to load texture : ") + p_filePaths[i])
            return;
        }

        if (IS_TEXTURE_LOADING_DEBUGGING_ON)
        {
            PRINT_MESSAGE_RUNTIME(std::string("Texture loaded successfully.") +
                "\n - File path : " + p_filePaths[i] + 
                "\n - Width     : " + std::to_string(_width) + 
                "\n - Height    : " + std::to_string(_height) + 
                "\n - BPP       : " + std::to_string(bitsPerPixel)
            )
        }
    }

    // Creating the OpenGL texture
    glGenTextures(1, &_inGpuId);
    glBindTexture(GL_TEXTURE_2D_ARRAY, _inGpuId);

    // Creating the texture 2D array
    // The 3D stands for the XY = TexturePosition (UV) and Z = TextureIndex (which texture you want to use)
    glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, _width, _height, _layerCount);

    // Upload each layer
    for (int i = 0; i < _layerCount; ++i)
    {
        glTexSubImage3D(
            GL_TEXTURE_2D_ARRAY,
            0,
            0,
            0,
            i,
            _width, _height, 1, GL_RGBA, GL_UNSIGNED_BYTE, layerData[i]
        );
        
        stbi_image_free(layerData[i]);
    }

    // - Setting parameters - //

    // Using 'GL_NEAREST' disable the texture smoothing (ideal for pixel art style)
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // Using 'GL_REPEAT' will make the texture repeat itself,
    // will be visible if we pass a vertex's texture position superior to 1
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);

    Unbind();
}

Texture2DArray::~Texture2DArray()
{
    // NOTE : We already delete the 'layerData' variable in the constructor so no need to delete it once more

    // Deleting texture from the GPU
    glDeleteTextures(1, &_inGpuId);
}

void Texture2DArray::Bind(const unsigned int p_textureSlot) const
{
    if (p_textureSlot > 31)
    {
        PRINT_ERROR_RUNTIME(true, "The given p_textureSlot is superior to 31, OpenGL only let us use 31 texture slots.")
        return;
    }
    
    glActiveTexture(GL_TEXTURE0 + p_textureSlot);
    glBindTexture(GL_TEXTURE_2D_ARRAY, _inGpuId);
}

void Texture2DArray::Unbind()
{
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}