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
    
    /// <summary>
    /// The Mask struct weight exactly 8 bytes, the same size as an address,
    /// that means you don't have to pass it as a reference. </summary>
    struct Mask
    {
        BlockTypes BlockType;
        int Normal;
    };

    // -- Chunk properties -- //  
    
    /// <summary> The world seed, changing it change how the word is generated. </summary>
    int WorldSeed = 1789;
    
    /// <summary> Frequency is used to create noise, which influences the steepness of slopes : the lower the frequency,
    /// the gentler the slopes, and the higher the frequency, the steeper the slopes. </summary>
    float NoiseFrequency = 0.03f;

    /// <summary> The chunk's 3D size. Enable the possibility to set the chunk's size to not be like a square. </summary>
    Vector3Uint Size = Vector3Uint(32, 32, 32);

    /// <summary> The shader that will be used to render chunk's vertices. </summary>
    Shader* RenderingShader;

private:
    
    FastNoiseLite* _noise;

    ChunkMeshData _meshData;
    std::vector<BlockTypes> _blocks;

    int	_vertexCount = 0;
    
public:

    // NOTE : We use "class Shader" to avoid conflict with the variable named Shader
    
    GreedyChunk(int p_worldSeed, float p_noiseFrequency, const Vector3Uint& p_size, Shader* p_renderingShader, bool p_doesInit = true);
    ~GreedyChunk();
    
    void Init();
    
    void SetBlockType(const Vector3Uint& p_blockPosition, const BlockTypes p_newBlockType);

    bool IsBlockOutsideChunk(const Vector3Uint& p_blockPosition) const;

private:

    void GenerateBlocks();

    void ClearMesh();
    
    void GenerateMesh();
    
    void ApplyMesh();


    // NOTE : The Mask struct weight exactly 8 bytes, the same size as an address,
    //        it's for this reason we don't pass it by const reference

    void CreateQuad(Mask p_mask, const Vector3Int& p_maskAxis, unsigned int p_width, unsigned int p_height,
        const Vector3Int& p_vertexPosition1, const Vector3Int& p_vertexPosition2, const Vector3Int& p_vertexPosition3, const Vector3Int& p_vertexPosition4);

    void SetBlockTypeData(const Vector3Uint& p_blockPosition, BlockTypes p_newBlockType);
    
    
    BlockTypes GetBlock(const Vector3Uint& p_blockPosition) const;
    
    int GetBlockIndex(const Vector3Uint& p_blockPosition) const;
    
    
    int GetEnvironmentTextureIndex(BlockTypes p_blockType, const Vector3& p_normal) const;
    
    bool IsSameMask(Mask p_mask1, Mask p_mask2) const;
};