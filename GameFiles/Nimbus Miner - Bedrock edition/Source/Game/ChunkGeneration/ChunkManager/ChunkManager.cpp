#include "ChunkManager.h"

#include <random>
#include <sstream>

#include "../GreedyChunk/GreedyChunk.h"

ChunkManager::ChunkManager(const bool p_isWorldSeedRandomized, int p_worldSeed, const float p_noiseFrequency,
    const Vector3Int& p_chunksSize, const int p_chunksBlockSize, const Vector2Int& p_chunkCount, Shader* p_renderingShader,
    const bool p_doesInit)
{
    if (p_isWorldSeedRandomized)
        p_worldSeed = GetRandomNumberInRange(0, 9999);

    // Setting up class' public variables
    WorldSeed = p_worldSeed;
    NoiseFrequency = p_noiseFrequency;
    ChunkSize = p_chunksSize;
    ChunksBlockSize = p_chunksBlockSize;
    ChunkCount = p_chunkCount;
    RenderingShader = p_renderingShader;

    if (p_doesInit)
        Init();
}

ChunkManager::~ChunkManager()
{
    for (const GreedyChunk* chunk : _generatedChunks)
        delete chunk;
    
    _generatedChunks.clear();
}

void ChunkManager::Init()
{
    // Changing the size (in bytes) of the _generatedChunks list to the exact number we need
    _generatedChunks.reserve(static_cast<long long>(ChunkCount.X) * ChunkCount.Y);

    for (int x = -ChunkCount.X; x < ChunkCount.X; ++x)
    {
        for (int z = -ChunkCount.Y; z < ChunkCount.Y; ++z)
        {
            // Computing the world position of the chunk
            Vector3 worldPosition = Vector3(
                static_cast<float>(x * ChunkSize.X),
                0.0f,
                static_cast<float>(z * ChunkSize.Z)
            );

            // Creating the chunk, and passing data to it
            GreedyChunk* newChunk = new GreedyChunk(
                worldPosition,
                WorldSeed,
                NoiseFrequency,
                ChunkSize,
                RenderingShader
            );

            _generatedChunks.push_back(newChunk);
        }
    }
}

void ChunkManager::DrawChunks() const
{
    for (const GreedyChunk* chunk : _generatedChunks)
        chunk->Draw();
}

GreedyChunk* ChunkManager::GetChunk(const Vector2Int& p_chunkIndex) const
{
    #pragma region Security
    
    if (IsOutsideChunks(p_chunkIndex))
    {
        std::stringstream errorMessage;
        errorMessage << "The given index ("
        << p_chunkIndex.X << ", " << p_chunkIndex.Y << ") is outside the chunk manager scope ("
        << ChunkCount.X - 1 << ", " << ChunkCount.Y - 1 << ")";

        PRINT_ERROR_RUNTIME(true, errorMessage.str())
        return nullptr;
    }
    #pragma endregion
    
    return _generatedChunks[p_chunkIndex.Y * ChunkCount.X + p_chunkIndex.X];
}

int ChunkManager::GetRandomNumberInRange(const int p_minimum, const int p_maximum)
{
    static std::random_device randomDevice;
    static std::mt19937 generator(randomDevice());
    
    std::uniform_int_distribution<> distribution(p_minimum, p_maximum);
    
    return distribution(generator);
}

bool ChunkManager::IsOutsideChunks(const Vector2Int& p_chunkIndex) const
{
    if (p_chunkIndex.X < -ChunkCount.X || p_chunkIndex.X > ChunkCount.X ||
        p_chunkIndex.Y < -ChunkCount.Y || p_chunkIndex.Y > ChunkCount.Y)
    {
        return true;
    }
    
    return false;
}