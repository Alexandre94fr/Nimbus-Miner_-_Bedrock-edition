#include "GreedyChunk.h"

#include <iostream>
#include <sstream>

#include "FastNoiseLite/FastNoiseLite.h"
#include "Renderer.h"

#include "DebuggingConstants.h"
#include "ProjectConstants.h"
#include "MessageDebugger/MessageDebugger.h"

GreedyChunk::GreedyChunk(const Vector3& p_worldPosition,
	const int p_worldSeed, const float p_noiseFrequency, const Vector3Int& p_size, Shader* p_renderingShader,
	const int p_blockPixelSize, const bool p_doesInit)
{
	// Initialising class' variables
	RenderingIndexBufferObject = nullptr;
	RenderingVertexArrayObject = nullptr;
	_noise = nullptr;
	_vertexBufferObject = nullptr;

    // Setting class' public variables
	WorldPosition = p_worldPosition;
    WorldSeed = p_worldSeed;
    NoiseFrequency = p_noiseFrequency;
    Size = p_size;
	BlockSize = p_blockPixelSize;
    RenderingShader = p_renderingShader;

    if (p_doesInit)
        Init();
}

GreedyChunk::~GreedyChunk()
{
    delete _noise;

	delete RenderingVertexArrayObject;
	delete RenderingIndexBufferObject;

	delete _vertexBufferObject;
}

void GreedyChunk::Init()
{
    // Creating the noise
    _noise = new FastNoiseLite(WorldSeed);
    _noise->SetFrequency(NoiseFrequency);
    _noise->SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    _noise->SetFractalType(FastNoiseLite::FractalType_FBm);

    // Changing the size (in bytes) of the _blocks list to the exact number we need
    _blocks.resize(static_cast<long long>(Size.X) * Size.Y * Size.Z);

    GenerateBlocks();

    GenerateMesh();

    UpdateDrawData();
}

void GreedyChunk::Draw() const
{
	Renderer::Draw(*RenderingVertexArrayObject, *RenderingIndexBufferObject, *RenderingShader);
}

void GreedyChunk::SetBlockType(const Vector3Int& p_blockPosition, const BlockTypes p_newBlockType)
{
    #pragma region Security
    
    if (IsBlockOutsideChunk(p_blockPosition))
    {
        std::stringstream errorMessage;
        errorMessage << "The given position ("
        << p_blockPosition.X << ", " << p_blockPosition.Y << ", " << p_blockPosition.Z << ") is outside the chunk scope ("
        << Size.X - 1 << ", " << Size.Y - 1 << ", " << Size.Z - 1 << ")";

        PRINT_ERROR_RUNTIME(true, errorMessage.str())
        return;
    }
    #pragma endregion
    
    SetBlockTypeData(p_blockPosition, p_newBlockType);

    // - Re-generate the chunk - //

    ClearMesh();

    GenerateMesh();

    UpdateDrawData();
}

bool GreedyChunk::IsBlockOutsideChunk(const Vector3Int& p_blockPosition) const
{
    if (p_blockPosition.X >= Size.X || p_blockPosition.Y >= Size.Y || p_blockPosition.Z >= Size.Z ||
        p_blockPosition.X < 0 || p_blockPosition.Y < 0 || p_blockPosition.Z < 0)
        return true;

    return false;
}

void GreedyChunk::GenerateBlocks()
{
	const Vector3 chunkLocation = WorldPosition;
	
	for (int x = 0; x < Size.X; x++)
	{
		for (int z = 0; z < Size.Z; z++)
		{
			const float xWorldPosition = (chunkLocation.X + static_cast<float>(x));
			const float zWorldPosition = (chunkLocation.Z + static_cast<float>(z));

			// Computing the height of the block (the data will be transfer to the const unsigned int "height" variable)
			int tempHeight = static_cast<int>(round((_noise->GetNoise(xWorldPosition, zWorldPosition) + 1) * static_cast<float>(Size.Y) / 2));

			// Clamping the height
			if (tempHeight < 0)
				tempHeight = 0;

			if (tempHeight > static_cast<int>(Size.Y))
				tempHeight = static_cast<int>(Size.Y);
			
			const int height = tempHeight;

			// We set all block's type by their height (compared to the ground)
			for (int y = 0; y < height; y++)
			{
				// NOTE : We begin by the last (the block more close to the bottom of the map)
				if (y < height - 30)
					_blocks[GetBlockIndex(Vector3Int(x, y, z))] = BlockTypes::ElectrifiedCloud;		// Ore

				else if (y < height - 29)
					_blocks[GetBlockIndex(Vector3Int(x, y, z))] = BlockTypes::DarkCloud;			// Environment

				else if (y < height - 25)
					_blocks[GetBlockIndex(Vector3Int(x, y, z))] = BlockTypes::VeryDarkCloud;		// Environment

				else if (y < height - 20)
					_blocks[GetBlockIndex(Vector3Int(x, y, z))] = BlockTypes::VeryVeryDarkCloud;	// Environment

				else if (y < height - 17)
					_blocks[GetBlockIndex(Vector3Int(x, y, z))] = BlockTypes::VeryDarkCloud;		// Environment

				else if (y < height - 16)
					_blocks[GetBlockIndex(Vector3Int(x, y, z))] = BlockTypes::DarkCloud;			// Environment

				else if (y < height - 15)
					_blocks[GetBlockIndex(Vector3Int(x, y, z))] = BlockTypes::NormalCloud;			// Environment

				else if (y == height - 15)
					_blocks[GetBlockIndex(Vector3Int(x, y, z))] = BlockTypes::HardCloud;			// Ore

				else if (y == height - 14)
					_blocks[GetBlockIndex(Vector3Int(x, y, z))] = BlockTypes::NormalCloud;			// Environment

				else if (y < height - 10)
					_blocks[GetBlockIndex(Vector3Int(x, y, z))] = BlockTypes::VeryVeryDarkCloud;	// Environment

				else if (y < height - 7)
					_blocks[GetBlockIndex(Vector3Int(x, y, z))] = BlockTypes::VeryDarkCloud;		// Environment

				else if (y < height - 4)
					_blocks[GetBlockIndex(Vector3Int(x, y, z))] = BlockTypes::DarkCloud;			// Environment

				else if (y < height - 1)
					_blocks[GetBlockIndex(Vector3Int(x, y, z))] = BlockTypes::NormalCloud;			// Environment

				else if (y == height - 1)
					_blocks[GetBlockIndex(Vector3Int(x, y, z))] = BlockTypes::LightCloud;			// Environment
			}

			// Otherwise them became air (everything that's higher than 'height' become Air)
			for (int y = height; y < Size.Y; y++)
				_blocks[GetBlockIndex(Vector3Int(x, y, z))] = BlockTypes::Air;
		}
	}
}

void GreedyChunk::ClearMesh()
{
    _vertexCount = 0;
    _meshData.Clear();
}

void GreedyChunk::GenerateMesh()
{
	// We go through each axis
	for (int axis = 0; axis < 3; ++axis)
	{
		// - Setting up locals variables - //

		// Returns the horizontal axis
		const int axis1 = (axis + 1) % 3;
		// Returns the vertical axis
		const int axis2 = (axis + 2) % 3; 

		const int mainAxisLimit = Size[axis];
		const int axis1Limit = Size[axis1];
		const int axis2Limit = Size[axis2];

		Vector3Int deltaAxis1 = Vector3Int::Zero();
		Vector3Int deltaAxis2 = Vector3Int::Zero();

		Vector3Int chunkIteration = Vector3Int::Zero();
		Vector3Int axisMask = Vector3Int::Zero();

		axisMask[axis] = 1;

		std::vector<Mask> masks;
		masks.resize(static_cast<unsigned long long>(axis1Limit) * axis2Limit);

		// Generating the slices
		for (chunkIteration[axis] = -1; chunkIteration[axis] < mainAxisLimit;)
		{
			int maskIteration = 0;

			// Filling the mask for the current slice
			for (chunkIteration[axis2] = 0; chunkIteration[axis2] < axis2Limit; ++chunkIteration[axis2])
			{	
				for (chunkIteration[axis1] = 0; chunkIteration[axis1] < axis1Limit; ++chunkIteration[axis1])
				{
					const BlockTypes currentBlock = GetBlock(chunkIteration);
					const BlockTypes comparedBlock = GetBlock(chunkIteration + axisMask);

					const bool isCurrentBlockOpaque = currentBlock != BlockTypes::Air;
					const bool isComparedBlockOpaque = comparedBlock != BlockTypes::Air;

					// If two opaque blocks are side by side we don't need to render the quad between them
					// because the player can't see it anyway #optimization
					if (isCurrentBlockOpaque == isComparedBlockOpaque)
					{
						masks[maskIteration++] = Mask{ BlockTypes::Null, 0 };
					}
					// If ONLY one of the two blocks are opaque we need to render the quad between them
					else if (isCurrentBlockOpaque)
					{
						// 1 = Forward
						masks[maskIteration++] = Mask{ currentBlock, 1 };
					}
					else
					{
						// -1 = Backward
						masks[maskIteration++] = Mask{ comparedBlock, -1 };
					}
				}
			}

			++chunkIteration[axis];

			// NOTE : We will need to iterate throw the mash again so we reset the 'maskIteration' variable
			maskIteration = 0;

			// Generate the mesh from the mask
			
			for (int y = 0; y < axis2Limit; ++y)
			{
				// NOTE :
				// Here the 'x' value will be increase manually
				for (int x = 0; x < axis1Limit;)
				{
					if (masks[maskIteration].Normal != 0)
					{
						const Mask currentMask = masks[maskIteration];

						chunkIteration[axis1] = x;
						chunkIteration[axis2] = y;

						// Represent the X size of the quad, over iteration this value will grow
						int width;
						int height;
						bool maxQuadSizeReached = false;

						#pragma region Documentation
						
						// EXAMPLE :
						// Lexicon :
						// - | = Chunk border
						// - ' = Air block type
						// - C = Cloud block type
						// - H = Hard cloud block type
						// - Q = Current quad that will be drawn as one block
						//		 (The quad as the Cloud block type)

						// 1.
						// | ' ' ' ' ' ' ' ' |
						// | ' ' ' ' ' H ' ' |
						// | ' ' ' C C C ' ' |
						// | ' ' H C C Q ' ' |
						
						// 2.
						// | ' ' ' ' ' ' ' ' |
						// | ' ' ' ' ' H ' ' |
						// | ' ' ' C C C ' ' |
						// | ' ' H C Q Q ' ' |

						// 3.
						// | ' ' ' ' ' ' ' ' |
						// | ' ' ' ' ' H ' ' |
						// | ' ' ' C C C ' ' |
						// | ' ' H Q Q Q ' ' |
						
						// 4.
						// | ' ' ' ' ' ' ' ' |
						// | ' ' ' ' ' H ' ' |
						// | ' ' ' C C C ' ' |
						// | ' ' H Q Q Q ' ' |

						#pragma endregion
						
						// All the logic appends in the for loop, it's normal if he doesn't have code inside
						for (width = 1; x + width < axis1Limit && IsSameMask(masks[maskIteration + width], currentMask); ++width) {}

						#pragma region Documentation
						
						// EXAMPLE :
						// Lexicon :
						// - | = Chunk border
						// - ' = Air block type
						// - C = Cloud block type
						// - H = Hard cloud block type
						// - Q = Current quad that will be drawn as one block
						//		 (The quad as the Cloud block type)

						// 1.
						// | ' ' ' ' ' ' ' ' |
						// | ' ' ' ' ' H ' ' |
						// | ' ' ' C C C ' ' |
						// | ' ' H Q Q Q ' ' |
						
						// 2.
						// | ' ' ' ' ' ' ' ' |
						// | ' ' ' ' ' H ' ' |
						// | ' ' ' Q Q Q ' ' |
						// | ' ' H Q Q Q ' ' |

						// 3.
						// | ' ' ' ' ' ' ' ' |
						// | ' ' ' ' ' H ' ' |
						// | ' ' ' Q Q Q ' ' |
						// | ' ' H Q Q Q ' ' |

						#pragma endregion
						
						for (height = 1; y + height < axis2Limit; ++height)
						{
							for (int i = 0; i < width; i++)
							{
								if (IsSameMask(masks[maskIteration + i + height * axis1Limit], currentMask))
									continue;

								maxQuadSizeReached = true;
								break;
							}

							if (maxQuadSizeReached)
								break;
						}

						deltaAxis1[axis1] = width;
						deltaAxis2[axis2] = height;

						CreateQuad(currentMask, axisMask,
							width,
							height,
							chunkIteration,
							chunkIteration + deltaAxis1,
							chunkIteration + deltaAxis2,
							chunkIteration + deltaAxis1 + deltaAxis2
						);

						// - Cleaning variables - //
						
						deltaAxis1 = Vector3Int::Zero();
						deltaAxis2 = Vector3Int::Zero();

						for (int j = 0; j < height; j++)
						{
							for (int i = 0; i < width; i++)
							{
								masks[maskIteration + i + j * axis1Limit] = Mask { BlockTypes::Null, 0 };
							}
						}
						//

						x += width;
						maskIteration += width;	
					}
					else
					{
						x++;
						maskIteration++;
					}
				}
			}
		}
	}
}

void GreedyChunk::UpdateDrawData()
{
	#pragma region Debugging
	
	if (IS_PRINTING_ALL_VERTICES_CREATED)
	{
		for (const Vertex vertex : _meshData.Vertices)
		{
			std::cout << "Vertex(Vector3(" << vertex.Position.X << ", " << vertex.Position.Y << ", " << vertex.Position.Z
			<< "), Vector3(" << vertex.Normal.X << ", " << vertex.Normal.Y << ", " << vertex.Normal.Z
			<< "), Vector4(" << vertex.Color.X << ", " << vertex.Color.Y << ", " << vertex.Color.Z << ", " << vertex.Color.W 
			<< "), Vector3(" << vertex.TexturePosition.X  << ", " << vertex.TexturePosition.Y << ", " << vertex.TexturePosition.Z << "))," << std::endl;
		}
	}

	if (IS_PRINTING_ALL_VERTICES_INDICES_CREATED)
	{
		for (int i = 0; i < _meshData.VerticesIndices.size() - 6; i += 6)
		{
			std::cout << _meshData.VerticesIndices[i] << ", " << _meshData.VerticesIndices[i + 1] << ", " << _meshData.VerticesIndices[i + 2] << ",\n"
			<< _meshData.VerticesIndices[i + 3] << ", " << _meshData.VerticesIndices[i + 4] << ", " << _meshData.VerticesIndices[i + 5] << ",\n" << std::endl;
		}
	}

	#pragma endregion
	
	// -- Resetting the RenderingVertexArrayObject, RenderingIndexBufferObject and _vertexBufferObject variables -- //
	
	if (RenderingVertexArrayObject != nullptr)
	{
		delete RenderingVertexArrayObject;
		RenderingVertexArrayObject = nullptr;
	}

	if (RenderingIndexBufferObject != nullptr)
	{
		delete RenderingIndexBufferObject;
		RenderingIndexBufferObject = nullptr;
	}

	if (_vertexBufferObject != nullptr)
	{
		delete _vertexBufferObject;
		_vertexBufferObject = nullptr;
	}
	
	// - Creating the VertexArrayObject - //
	
	VertexBufferLayoutObject vertexBufferLayoutObject;
	vertexBufferLayoutObject.PushBack<float>(3, false); // Represent the position
	vertexBufferLayoutObject.PushBack<float>(3, false); // Represent the normal
	vertexBufferLayoutObject.PushBack<float>(4, false); // Represent the color
	vertexBufferLayoutObject.PushBack<float>(3, false); // Represent the texture position (UV) and TextureIndex (Layer)
														// [the index of which texture will be drawn]

	_vertexBufferObject = new VertexBufferObject(
		ConvertVerticesToFloatArray(_meshData.Vertices).data(),
		sizeof(Vertex) * static_cast<unsigned int>(_meshData.Vertices.size())
	);

	RenderingVertexArrayObject = new VertexArrayObject();
	RenderingVertexArrayObject->AddBuffer(*_vertexBufferObject, vertexBufferLayoutObject);

	// - Creating the IndexBufferObject - //
	
	unsigned int verticesIndicesCount = 0;
	const unsigned int* verticesIndices = ChunkMeshData::ConvertVectorToArray(_meshData.VerticesIndices, verticesIndicesCount);
	
	RenderingIndexBufferObject = new IndexBufferObject(verticesIndices, verticesIndicesCount);
	
	// - Avoiding memory leaks (my beloved <3) - //
	delete[] verticesIndices;
}

void GreedyChunk::CreateQuad(const Mask p_mask, const Vector3Int& p_maskAxis, const unsigned int p_width, const unsigned int p_height,
    const Vector3& p_vertexPosition1, const Vector3& p_vertexPosition2, const Vector3& p_vertexPosition3, const Vector3& p_vertexPosition4)
{
	const Vector3Int quadNormal = Vector3Int(p_maskAxis * p_mask.Normal);

	const unsigned int textureIndex = GetEnvironmentTextureIndex(p_mask.BlockType, quadNormal);

	// -- Computing the texture color -- //

	// Depending on the quad orientation we will change the texture color (making it darker)

	// Top
    Vector4 color = CHUNK_BLOCK_TOP_TEXTURE_COLOR;

	// Left, right, forward, backward
	if (quadNormal.Y == 0)
		color = CHUNK_BLOCK_SIDE_TEXTURE_COLOR;

	// Down
	if (quadNormal.Y == -1)
		color = CHUNK_BLOCK_BOTTOM_TEXTURE_COLOR;

	// - Computing texture positions - //
	
	Vector3Uint texturePosition1;
	Vector3Uint texturePosition2;
	Vector3Uint texturePosition3;
	Vector3Uint texturePosition4;
	
	// To avoid having our sprite rotated (it's because of the way we handle the vertices indices)
	// we adapt our texture positions (UVs)
	if (quadNormal.X == 1 || quadNormal.X == -1)
	{
		texturePosition1 = Vector3Uint(p_width,		p_height,	 textureIndex);
		texturePosition2 = Vector3Uint(0,			p_height,	 textureIndex);
		texturePosition3 = Vector3Uint(p_width,		0,			 textureIndex);
		texturePosition4 = Vector3Uint(0,			0,			 textureIndex);
	}
	else
	{
		texturePosition1 = Vector3Uint(p_height,	p_width,	 textureIndex);
		texturePosition2 = Vector3Uint(p_height,	0,			 textureIndex);
		texturePosition3 = Vector3Uint(0,			p_width,	 textureIndex);
		texturePosition4 = Vector3Uint(0,			0,			 textureIndex);
	}

	// - Adding vertices into the mesh data - //
	
	_meshData.Vertices.emplace_back((p_vertexPosition1 + WorldPosition) * BlockSize, quadNormal, color, texturePosition1);
	_meshData.Vertices.emplace_back((p_vertexPosition2 + WorldPosition) * BlockSize, quadNormal, color, texturePosition2);
	_meshData.Vertices.emplace_back((p_vertexPosition3 + WorldPosition) * BlockSize, quadNormal, color, texturePosition3);
	_meshData.Vertices.emplace_back((p_vertexPosition4 + WorldPosition) * BlockSize, quadNormal, color, texturePosition4);
	
	// - Computing vertices drawing order - //
	
	// First triangle
	_meshData.VerticesIndices.push_back(_vertexCount);						// down-left
	_meshData.VerticesIndices.push_back(_vertexCount + 2 - p_mask.Normal);	// down-right
	_meshData.VerticesIndices.push_back(_vertexCount + 2 + p_mask.Normal);	// up-right
	// Second triangle
	_meshData.VerticesIndices.push_back(_vertexCount + 3);					// up-right
	_meshData.VerticesIndices.push_back(_vertexCount + 1 + p_mask.Normal);	// up-left
	_meshData.VerticesIndices.push_back(_vertexCount + 1 - p_mask.Normal);	// down-left

	// NOTES :
	// - We use 'p_mask.Normal' to get the correct orientation for our quad
	// - When the chunk will be fully generated,
	// we will use the '_meshData.VerticesIndices' to create a IndexBufferObject and pass it to the Renderer class

	_vertexCount += 4;
}

void GreedyChunk::SetBlockTypeData(const Vector3Int& p_blockPosition, const BlockTypes p_newBlockType)
{
    _blocks[GetBlockIndex(Vector3Int(p_blockPosition.X, p_blockPosition.Y, p_blockPosition.Z))] = p_newBlockType;
}

BlockTypes GreedyChunk::GetBlock(const Vector3Int& p_blockPosition) const
{
	if (IsBlockOutsideChunk(p_blockPosition))
		return BlockTypes::Air;

	// Return the type of block at the wanted position
	return _blocks[GetBlockIndex(p_blockPosition)];
}

unsigned int GreedyChunk::GetBlockIndex(const Vector3Int& p_blockPosition) const
{
	return p_blockPosition.X + Size.X * (p_blockPosition.Y + Size.Y * p_blockPosition.Z);
}

unsigned int GreedyChunk::GetEnvironmentTextureIndex(const BlockTypes p_blockType, const Vector3& p_normal) const
{
	switch (p_blockType)
	{
		// Environment
		case BlockTypes::Null:	return 0;
		case BlockTypes::Air:	return 0;
		
		case BlockTypes::LightCloud:
	
			if (p_normal == Vector3::Down())
				return 1;
			
			return 0;
	
		case BlockTypes::NormalCloud:		return 1;
		case BlockTypes::DarkCloud:			return 2;
		case BlockTypes::VeryDarkCloud:		return 3;
		case BlockTypes::VeryVeryDarkCloud: return 4;
	
		// Ores
		case BlockTypes::HardCloud:			return 5;
		case BlockTypes::ElectrifiedCloud:	return 6;
	
		default:
			std::stringstream errorMessage;
			errorMessage << "The given block's type '" << static_cast<int>(p_blockType) << "' (enum ID) in not planned the switch";
	
			PRINT_ERROR_RUNTIME(true, errorMessage.str())
			return 255; // Why 255 ? Because for colors 255 is the maximum number you can pass

	}
}

bool GreedyChunk::IsSameMask(const Mask p_mask1, const Mask p_mask2) const
{
	return p_mask1.BlockType == p_mask2.BlockType && p_mask1.Normal == p_mask2.Normal;
}