#pragma once

#include <string>
#include <vector>

class Texture2DArray
{

private:
    unsigned int _inGpuId;
    int _width;
    int _height;
    int _layerCount;

public:
    Texture2DArray(const std::vector<std::string>& p_filePaths);
    ~Texture2DArray();

    void Bind(const unsigned int p_textureSlot = 0) const;
    static void Unbind();
};
