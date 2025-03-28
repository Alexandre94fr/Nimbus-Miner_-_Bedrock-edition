#pragma once

// -- Application.cpp constants -- //

// NOTE : Those float variable are floats to avoid conversion,
//		  put please don't pass floating values.

static constexpr float WINDOW_SIZE_X = 640;
static constexpr float WINDOW_SIZE_Y = 480;
static constexpr float WINDOW_SIZE_Z = 1000;

static constexpr float CAMERA_FRUSTUM_NEAR = 0.05f;
static constexpr float CAMERA_FRUSTUM_FAR = 10000.0f;