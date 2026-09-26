#pragma once

#include "math/matrix.hpp"

struct Vertex {
  v3f position;
  v3f color;
  v2f texture;
};

// clang-format off
constexpr std::array<v3f, 36> CUBE_VERTICES{
    // Front face of cube - Red
    vec3(-0.5f, -0.5f, 0.5f), vec3(-0.5f, 0.5f, 0.5f), vec3(0.5f, 0.5f, 0.5f), 
    vec3(-0.5f, -0.5f, 0.5f), vec3(0.5f, 0.5f, 0.5f), vec3(0.5f, -0.5f, 0.5f),
    // Back face of cube 
    vec3(-0.5f, -0.5f, -0.5f), vec3(-0.5f, 0.5f, -0.5f), vec3(0.5f, 0.5f, -0.5f),
    vec3(-0.5f, -0.5f, -0.5f), vec3(0.5f, 0.5f, -0.5f), vec3(0.5f, -0.5f, -0.5f),
    // Floor - Blue
    // Assumes location is valid
    vec3(-0.5f, -0.5f, -0.5f), vec3(-0.5f, -0.5f, 0.5f), vec3(0.5f, -0.5f, 0.5f),
    vec3(-0.5f, -0.5f, -0.5f), vec3(0.5f, -0.5f, -0.5f), vec3(0.5f, -0.5f, 0.5f),
    // Ceil - Yellow
    vec3(-0.5f, 0.5f, -0.5f), vec3(-0.5f, 0.5f, 0.5f), vec3(0.5f, 0.5f, 0.5f), 
    vec3(-0.5f, 0.5f, -0.5f), vec3(0.5f, 0.5f, -0.5f), vec3(0.5f, 0.5f, 0.5f), 
    // Left wall - Purple
    vec3(-0.5f, -0.5f, 0.5f), vec3(-0.5f, -0.5f, -0.5f), vec3(-0.5f, 0.5f, -0.5f),
    vec3(-0.5f, -0.5f, 0.5f), vec3(-0.5f, 0.5f, 0.5f), vec3(-0.5f, 0.5f, -0.5f),
    // Right wall - White
    vec3(0.5f, -0.5f, 0.5f), vec3(0.5f, -0.5f, -0.5f), vec3(0.5f, 0.5f, -0.5f),
    vec3(0.5f, -0.5f, 0.5f), vec3(0.5f, 0.5f, 0.5f), vec3(0.5f, 0.5f, -0.5f),
};

// cube texture UV coordinates
constexpr std::array<v2f,36> CUBE_UV{
   // Front face
    vec2(0.0f, 0.0f), vec2(0.0f, 1.0f), vec2(1.0f, 1.0f),
    vec2(0.0f, 0.0f), vec2(1.0f, 1.0f), vec2(1.0f, 0.0f),

    // Back face
    vec2(0.0f, 0.0f), vec2(0.0f, 1.0f), vec2(1.0f, 1.0f),
    vec2(0.0f, 0.0f), vec2(1.0f, 1.0f), vec2(1.0f, 0.0f),

    // Floor (Bottom)
    vec2(0.0f, 1.0f), vec2(0.0f, 0.0f), vec2(1.0f, 0.0f),
    vec2(0.0f, 1.0f), vec2(1.0f, 1.0f), vec2(1.0f, 0.0f),

    // Ceil (Top)
    vec2(0.0f, 1.0f), vec2(0.0f, 0.0f), vec2(1.0f, 0.0f),
    vec2(0.0f, 1.0f), vec2(1.0f, 1.0f), vec2(1.0f, 0.0f),

    // Left wall
    vec2(1.0f, 0.0f), vec2(0.0f, 0.0f), vec2(0.0f, 1.0f),
    vec2(1.0f, 0.0f), vec2(1.0f, 1.0f), vec2(0.0f, 1.0f),

    // Right wall
    vec2(0.0f, 0.0f), vec2(1.0f, 0.0f), vec2(1.0f, 1.0f),
    vec2(0.0f, 0.0f), vec2(0.0f, 1.0f), vec2(1.0f, 1.0f),
};

constexpr std::array<v2f,6> SQUARE_VERTICES {
   vec2(-0.5f,-0.5f),
   vec2(-0.5f,0.5f),
   vec2(0.5f,0.5f),

   vec2(0.5f,0.5f),
   vec2(-0.5f,-0.5f),
   vec2(0.5f,-0.5f),
};
// clang-format on
