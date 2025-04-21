#pragma once

#include <vector>

#include "Shader.h"
#include "VertexArrayObject.h"

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

    Vector3 WorldPosition;
    
    /// <summary> The world seed, changing it change how the word is generated. </summary>
    int WorldSeed = 1789;
    
    /// <summary> Frequency is used to create noise, which influences the steepness of slopes : the lower the frequency,
    /// the gentler the slopes, and the higher the frequency, the steeper the slopes. </summary>
    float NoiseFrequency = 0.03f;

    /// <summary> The chunk's 3D size. Enable the possibility to set the chunk's size to not be like a square. </summary>
    Vector3Int Size = Vector3Int(32, 32, 32);

    int BlockSize = 1;

    // - Rendering - //
    
    /// <summary> The VertexArrayObject that will be used to render chunk's vertices. </summary>
    VertexArrayObject* RenderingVertexArrayObject;

    /// <summary> The IndexBufferObject that will be used to render chunk's vertices. </summary>
    IndexBufferObject* RenderingIndexBufferObject;
    
    /// <summary> The shader that will be used to render chunk's vertices. </summary>
    Shader* RenderingShader;

private:
    
    FastNoiseLite* _noise;

    ChunkMeshData _meshData;
    std::vector<BlockTypes> _blocks;

    int	_vertexCount = 0;

    VertexBufferObject* _vertexBufferObject;
    
public:
    
    GreedyChunk(const Vector3& p_worldPosition,
        const int p_worldSeed, const float p_noiseFrequency, const Vector3Int& p_size, Shader* p_renderingShader,
        const int p_blockPixelSize = 1, const bool p_doesInit = true);
    ~GreedyChunk();
    
    void Init();

    void Draw() const;
    
    void SetBlockType(const Vector3Int& p_blockPosition, const BlockTypes p_newBlockType);

    bool IsBlockOutsideChunk(const Vector3Int& p_blockPosition) const;

private:

    void GenerateBlocks();

    void ClearMesh();
    
    void GenerateMesh();
    
    void UpdateDrawData();


    // NOTE : The Mask struct weight exactly 8 bytes, the same size as an address,
    //        it's for this reason we don't pass it by const reference

    void CreateQuad(const Mask p_mask, const Vector3Int& p_maskAxis, const unsigned int p_width, const unsigned int p_height,
        const Vector3& p_vertexPosition1, const Vector3& p_vertexPosition2, const Vector3& p_vertexPosition3, const Vector3& p_vertexPosition4);

    void SetBlockTypeData(const Vector3Int& p_blockPosition, const BlockTypes p_newBlockType);
    
    
    BlockTypes GetBlock(const Vector3Int& p_blockPosition) const;
    
    unsigned int GetBlockIndex(const Vector3Int& p_blockPosition) const;
    
    /// <summary> Returns the index of a texture inside a Texture array. </summary>
    unsigned int GetEnvironmentTextureIndex(const BlockTypes p_blockType, const Vector3& p_normal) const;
    
    bool IsSameMask(const Mask p_mask1, const Mask p_mask2) const;
};