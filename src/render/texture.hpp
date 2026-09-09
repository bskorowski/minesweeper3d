#pragma once
#include "core.hpp"
#include "glad.h"
#include <cstdint>

struct TextureParams {
  // Behaviour in case of texture 'S' (Horizontal) axis cooridantes are out of
  // bounds
  int horizontalWrap = GL_MIRRORED_REPEAT;
  // Behaviour in case of texture 'T' (Vertical) axis cooridantes are out of
  // bounds
  int verticalWrap = GL_MIRRORED_REPEAT;

  // Filtering
  int minFilter = GL_LINEAR;

  // Filtering
  int magFilter = GL_LINEAR;
};

struct Texture {

public:
  // Creates a texture from given data. assumes the data is a valid buffer.
  // RGB only
  Texture(std::uint8_t *data, int width, int height,
          TextureParams params = {}) noexcept;
  constexpr Texture() noexcept = default;
  Texture(Texture &&);
  Texture &operator=(Texture &&);
  Texture(const Texture &) = delete;
  Texture &operator=(const Texture &) = delete;
  ~Texture();
  void generateMipMaps();

public:
  GLuint ID = render::UNSET;
};

struct TextureArray {
public:
  TextureArray(int width, int height, int layers,
               TextureParams params = {}) noexcept;
  constexpr TextureArray() noexcept = default;
  TextureArray(TextureArray &&);
  TextureArray &operator=(TextureArray &&);
  TextureArray(const TextureArray &) = delete;
  TextureArray &operator=(const TextureArray &) = delete;
  ~TextureArray() noexcept;

  void bind();

  // The texture must be bound before usage
  // All added textures should have the same size and height
  void addTexture(std::uint8_t *data, int layer);
  // The texture must be bound before usage
  void generateMipMaps();

public:
  GLuint ID = render::UNSET;
  int width = render::UNSET;
  int height = render::UNSET;
};
