#pragma once

// Represent the block's face direction
enum class Directions
{
    Up,
    Down,
    Left,
    Right,
    Forward,
    Back
};

// Represent all block's types
enum class BlockTypes : uint8_t
{
    Null,

    // -- Transparent blocks -- //
    Air,

    // -- Opaque blocks -- //

    // Environment
    LightCloud,
    NormalCloud,
    DarkCloud,
    VeryDarkCloud,
    VeryVeryDarkCloud,

    // Resources
    HardCloud,
    ElectrifiedCloud
};