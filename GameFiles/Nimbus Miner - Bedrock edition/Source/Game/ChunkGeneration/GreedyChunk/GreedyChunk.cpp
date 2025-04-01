#include "GreedyChunk.h"

#include <sstream>

#include "FastNoiseLite/FastNoiseLite.h"
#include "MessageDebugger/MessageDebugger.h"

GreedyChunk::GreedyChunk(int p_worldSeed, float p_noiseFrequency, const Vector3Uint& p_size, class Shader* p_renderingShader, bool p_doesInit)
{
    // Setting class' public variables
    WorldSeed = p_worldSeed;
    NoiseFrequency = p_noiseFrequency;
    Size = p_size;
    RenderingShader = p_renderingShader;

    if (p_doesInit)
        Init();
}

GreedyChunk::~GreedyChunk()
{
    delete _noise;
}

void GreedyChunk::Init()
{
    // Creating the noise
    _noise = new FastNoiseLite(WorldSeed);
    _noise->SetFrequency(NoiseFrequency);
    _noise->SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    _noise->SetFractalType(FastNoiseLite::FractalType_FBm);

    // Changing the size (in bytes) of the _blocks list to the exact number we need
    _blocks.reserve(sizeof(int) * Size.X * Size.Y * Size.Z);

    GenerateBlocks();

    GenerateMesh();

    ApplyMesh();
}

void GreedyChunk::SetBlockType(const Vector3Uint& p_blockPosition, const BlockTypes p_newBlockType)
{
    // Security
    if (IsBlockOutsideChunk(p_blockPosition))
    {
        std::stringstream errorMessage;
        errorMessage << "The given position ("
        << p_blockPosition.X << ", " << p_blockPosition.Y << ", " << p_blockPosition.Z << ") is outside the chunk scope ("
        << Size.X - 1 << ", " << Size.Y - 1 << ", " << Size.Z - 1 << ")";

        PRINT_ERROR_RUNTIME(true, errorMessage.str())
        return;
    }

    SetBlockTypeData(p_blockPosition, p_newBlockType);

    // Re-generate the chunk
    ClearMesh();

    GenerateMesh();

    ApplyMesh();
}

bool GreedyChunk::IsBlockOutsideChunk(const Vector3Uint& p_blockPosition) const
{
    if (p_blockPosition.X >= Size.X || p_blockPosition.Y >= Size.Y || p_blockPosition.Z >= Size.Z ||
        p_blockPosition.X < 0 || p_blockPosition.Y < 0 || p_blockPosition.Z < 0)
        return true;

    return false;
}

void GreedyChunk::GenerateBlocks()
{
    
}

void GreedyChunk::ClearMesh()
{
    
}

void GreedyChunk::GenerateMesh()
{
    
}

void GreedyChunk::ApplyMesh()
{
    
}

void GreedyChunk::CreateQuad(Mask p_mask, const Vector3Int& p_maskAxis, unsigned int p_width, unsigned int p_height,
    const Vector3Int& p_vertexPosition1, const Vector3Int& p_vertexPosition2, const Vector3Int& p_vertexPosition3, const Vector3Int& p_vertexPosition4)
{
    
}

void GreedyChunk::SetBlockTypeData(const Vector3Uint& p_blockPosition, BlockTypes p_newBlockType)
{
    
}

BlockTypes GreedyChunk::GetBlock(const Vector3Uint& p_blockPosition) const
{
    return BlockTypes::Air;
}

int GreedyChunk::GetBlockIndex(const Vector3Uint& p_blockPosition) const
{
    return false;
}

int GreedyChunk::GetEnvironmentTextureIndex(BlockTypes p_blockType, const Vector3& p_normal) const
{
    return false;
}

bool GreedyChunk::IsSameMask(Mask p_mask1, Mask p_mask2) const
{
    return false;
}