#pragma once

#include <vector>

#include "Shader.h"

#include "../ChunkMeshData.h"
#include "../EnvironmentEnums.h"

// Forward declaration
class FastNoiseLite;

class GreedyChunk
{
    
public:

    struct FMask
    {
        BlockTypes BlockType;
        int Normal;
    };

    // -- Chunk properties -- //  
    
    /// <summary> The world seed, changing it change how the word is generated. </summary>
    unsigned int WorldSeed = 1789;
    
    /// <summary> Frequency is used to create noise, which influences the steepness of slopes : the lower the frequency,
    /// the gentler the slopes, and the higher the frequency, the steeper the slopes. </summary>
    float NoiseFrequency = 0.03f;

    // <summary> The chunk's 3D size. Enable the possibility to set the chunk's size to not be like a square. </summary>
    Vector3 Size = Vector3(32, 32, 32);

    /// <summary> The shader that will be used to render chunk's vertices. </summary>
    Shader* Shader;

private:
    
    FastNoiseLite* _noise;

    ChunkMeshData _meshData;
    std::vector<BlockTypes> _blocks;

    int	_vertexCount = 0;
    
public:

    GreedyChunk(unsigned int p_worldSeed, float p_noiseFrequency, float p_size, const class Shader& p_shader, bool p_doesInit = true);
    
    void Init();
    
    void SetBlockType(const Vector3& p_blockPosition, BlockTypes p_newBlockType);

    bool IsBlockOutsideChunk(const Vector3& p_blockPosition);

private:

    void GenerateBlocks();

    void ClearMesh();
    
    void GenerateMesh();
    
    void ApplyMesh();


    void CreateQuad(FMask p_mask, Vector3 p_maskAxis, unsigned int p_width, unsigned int p_height,
        const Vector3& p_faceVertexes1, const Vector3& p_faceVertexes2, const Vector3& p_faceVertexes3, const Vector3& p_faceVertexes4);

    void SetBlockTypeData(const Vector3& p_blockPosition, BlockTypes p_newBlockType);
    
    
    BlockTypes GetBlock(const Vector3& p_index) const;
    
    int GetBlockIndex(unsigned int p_xPosition, unsigned int p_yPosition, unsigned int p_zPosition) const;
    
    
    int GetEnvironmentTextureIndex(BlockTypes p_blockType, const Vector3& p_normal) const;
    
    bool CompareMask(FMask p_mask1, FMask p_mask2) const;
};