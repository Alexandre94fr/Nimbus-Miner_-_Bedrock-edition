#pragma once

#include <string>

class Texture
{

private:

    unsigned int _inGpuId;
    
    std::string _filePath; 
    
    int _width;
    int _height;
    int _bitsPerPixel;
    
    // Represent the PNG pixels data (RGBA)
    unsigned char* _localDataBuffer; 
    
public:

    Texture(const std::string& p_filePath);
    ~Texture();
    
    void Bind(const unsigned int p_textureSlot = 0) const;
    static void Unbind();

    // Getters
    
    unsigned int GetInGpuId() const { return _inGpuId; }
    
    std::string GetFilePath() const { return _filePath; }
    
    int GetWidth() const { return _width; }
    int GetHeight() const { return _height; }
    int GetBitsPerPixel() const { return _bitsPerPixel; }
    
};