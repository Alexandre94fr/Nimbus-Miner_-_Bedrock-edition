#pragma once

#include <vector>

#include "Vector.h"

class Shader;
class GreedyChunk;

class ChunkManager
{
    
public:
    
    /// <summary> The world seed, changing it change how the word is generated. </summary>
    int WorldSeed = 1789;
    
    /// <summary> Frequency is used to create noise, which influences the steepness of slopes : the lower the frequency,
    /// the gentler the slopes, and the higher the frequency, the steeper the slopes. </summary>
    float NoiseFrequency = 0.03f;

    /// <summary> Represents the 3D size of all chunks. Enables the possibility to set the chunk's size to not be like a square. </summary>
    Vector3Int ChunkSize = Vector3Int(32, 32, 32);

    /// <summary> The size of the block (does not have a unit, but you can consider it has a meter). </summary>
    int ChunksBlockSize = 1;
    
    /// <summary>
    /// Represents the number of chunks that will be generated <b> around </b> the (0, 0, 0) of the world.
    /// <para> That means if you have ChunkCount = Vector2Int(10, 10), 400 chunks will be generated (10 * 10 * 4 or 20 * 20). </para> </summary>
    Vector2Int ChunkCount = Vector2Int(10, 10);
    
    /// <summary>
    /// The common shader for all the generated chunks.
    /// <para> The shader that will be used to render chunk's vertices. </para> </summary>
    Shader* RenderingShader = nullptr;

private:

    std::vector<GreedyChunk*> _generatedChunks;
    
public:
    
    ChunkManager(const bool p_isWorldSeedRandomized, int p_worldSeed, const float p_noiseFrequency,
        const Vector3Int& p_chunksSize, const int p_chunksBlockSize, const Vector2Int& p_chunkCount, Shader* p_renderingShader,
        const bool p_doesInit = true);
    ~ChunkManager();

    void Init();

    void DrawChunks() const;
    
    GreedyChunk* GetChunk(const Vector2Int& p_chunkIndex) const;
    
private:
    
    static int GetRandomNumberInRange(const int p_minimum, const int p_maximum);
    bool IsOutsideChunks(const Vector2Int& p_chunkIndex) const;
};