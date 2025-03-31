#pragma once

// -=- Application.cpp constants -=- //

// NOTE : Those float variable are floats to avoid conversion,
//		  put please don't pass floating values.

static constexpr float WINDOW_SIZE_X = 640;
static constexpr float WINDOW_SIZE_Y = 480;
static constexpr float WINDOW_SIZE_Z = 1000;

// -- Camera -- //

static constexpr glm::vec3 CAMERA_SPAWN_POSITION = glm::vec3(0, 0, 3);
static constexpr float CAMERA_MOVEMENT_SPEED = 1.0f;
static constexpr float CAMERA_ROTATION_SENSITIVITY = 0.1f;

static constexpr float CAMERA_FRUSTUM_NEAR = 0.05f;
static constexpr float CAMERA_FRUSTUM_FAR = 10000.0f;