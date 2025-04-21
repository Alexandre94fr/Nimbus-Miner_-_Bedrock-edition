#pragma once

// -=- Application.cpp constants -=- //

// NOTE : Those float variable are floats to avoid conversion,
//		  put please don't pass floating values.

static constexpr float WINDOW_SIZE_X = 1280;
static constexpr float WINDOW_SIZE_Y = 960;
static constexpr float WINDOW_SIZE_Z = 1000;

// -- Camera -- //

static constexpr glm::vec3 CAMERA_SPAWN_POSITION = glm::vec3(0, 50, 10);

static constexpr float CAMERA_MOVEMENT_SPEED = 1.0f;
static constexpr float CAMERA_MOVEMENT_SPEED_MULTIPLICATOR = 10.0f; 
// In order to keep the CAMERA_MOVEMENT_SPEED relatively low we use this constant

static constexpr float CAMERA_ROTATION_SENSITIVITY = 0.1f;

static constexpr float CAMERA_FRUSTUM_NEAR = 0.05f;
static constexpr float CAMERA_FRUSTUM_FAR  = 10000.0f;

// -=- Render.cpp constants -=- //

static constexpr glm::vec4 BACKGROUND_COLOR = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);

// -=- GreedyChunk.cpp constants -=- //

static constexpr glm::vec4 CHUNK_BLOCK_TOP_TEXTURE_COLOR	= glm::vec4(1.00f, 1.00f, 1.00f, 1.00f);
static constexpr glm::vec4 CHUNK_BLOCK_SIDE_TEXTURE_COLOR	= glm::vec4(0.85f, 0.85f, 0.85f, 1.00f);
static constexpr glm::vec4 CHUNK_BLOCK_BOTTOM_TEXTURE_COLOR = glm::vec4(0.70f, 0.70f, 0.70f, 1.00f);