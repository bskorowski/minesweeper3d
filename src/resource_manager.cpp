#include "resource_manager.hpp"
#include "debug_utils.hpp"
#include "render/texture.hpp"
#include <filesystem>
#include <logzy/logzy.hpp>
#include <span>
#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

auto ResourceManager::loadTexture(ResourceKey resourceKey,
                                  std::string_view assetPath,
                                  TextureParams params,
                                  std::filesystem::path basePath) -> bool {

  if (textures_.contains(resourceKey)) {
    logzy::debug("Texture already loaded: '{}'", resourceKey);
    return true;
  }

  std::filesystem::path fullPath = basePath / assetPath;
  logzy::info("Loading texture at: '{}'", fullPath);

  if (!verifyPath(fullPath)) {
    return false;
  }

  int width = -1;
  int height = -1;
  int channels = -1;
  constexpr int desiredChannels = 3;

  stbi_set_flip_vertically_on_load(true);

  auto *data = stbi_load(fullPath.string().c_str(), &width, &height, &channels,
                         desiredChannels);

  if (data == nullptr) {
    logzy::critical("Couldn't find texture: {}", fullPath.string());
  }

  DEBUG_ASSERT(width > 0, std::format("Actual width: {}", width));
  DEBUG_ASSERT(height > 0, std::format("Actual width: {}", height));
  DEBUG_ASSERT(
      channels == desiredChannels,
      std::format("channels={} desired={}", channels, desiredChannels));

  Texture texture(data, width, height);
  texture.generateMipMaps();

  ResourceManager::textures_.emplace(resourceKey, std::move(texture));
  //
  stbi_image_free(data);
  return true;
}

auto ResourceManager::unloadTexture(ResourceKey resourceKey) -> bool {
  DEBUG_ASSERT(
      textures_.contains(resourceKey),
      std::format("Texture '{}' must be loaded to unload it", resourceKey));

  if (!textures_.erase(resourceKey)) {
    logzy::warn("Couldn't unload texture {}", resourceKey);
    return false;
  }
  logzy::trace("Unloaded texture {}", resourceKey);
  return true;
}

auto ResourceManager::getTexture(ResourceKey resourceKey) -> const Texture & {
  DEBUG_ASSERT(
      textures_.contains(resourceKey),
      std::format("Texture '{}' must be loaded to use it", resourceKey));
  return textures_.at(resourceKey);
}

auto ResourceManager::loadTextureArray(ResourceKey resourceKey,
                                       std::span<const std::string_view> paths,
                                       TextureParams params,
                                       std::filesystem::path basePath) -> bool {

  if (textureArrays_.contains(resourceKey)) {
    logzy::debug("Texture already loaded: '{}'", resourceKey);
    return true;
  }
  DEBUG_ASSERT(paths.size() > 0);

  int layer = 0;
  // Manually doing the first as a "blueprint" for height and width and channels
  std::string_view first = paths[0];
  paths = paths.subspan(1, std::dynamic_extent);

  std::filesystem::path currentPath = basePath / first;

  int referenceWidth = -1;
  int referenceHeight = -1;
  int referenceChannels = -1;
  constexpr int desiredChannels = 3;

  auto *data = stbi_load(currentPath.string().c_str(), &referenceWidth,
                         &referenceHeight, &referenceChannels, desiredChannels);

  DEBUG_ASSERT(data != nullptr);
  DEBUG_ASSERT(referenceWidth > 0,
               std::format("Actual width: {}", referenceWidth));
  DEBUG_ASSERT(referenceHeight > 0,
               std::format("Actual width: {}", referenceHeight));
  DEBUG_ASSERT(referenceChannels == desiredChannels,
               std::format("channels={} desired={}", referenceChannels,
                           desiredChannels));

  TextureArray array(referenceWidth, referenceHeight, paths.size() + 1, params);
  array.bind();

  stbi_set_flip_vertically_on_load(true);

  array.addTexture(data, layer++);
  stbi_image_free(data);

  for (std::string_view pathPart : paths) {
    std::filesystem::path currentPath = basePath / pathPart;
    DEBUG_ONLY(logzy::info("Loading texture at: '{}'", currentPath));
    if (!verifyPath(currentPath)) {
      logzy::critical("Texture not found: {}, couldn't load layer: {}",
                      currentPath.string().c_str(), layer++);
      return false;
    }

    int width = -1;
    int height = -1;
    int channels = -1;

    auto *data = stbi_load(currentPath.string().c_str(), &width, &height,
                           &channels, desiredChannels);

    DEBUG_ASSERT(data != nullptr);
    DEBUG_ASSERT(referenceWidth == width,
                 std::format("{} == {}", referenceWidth, width));
    DEBUG_ASSERT(referenceHeight == height,
                 std::format("{} == {}", referenceHeight, height));
    DEBUG_ASSERT(referenceChannels == channels,
                 std::format("{} == {}", referenceChannels, channels));

    array.addTexture(data, layer++);
    stbi_image_free(data);
  }

  ResourceManager::textureArrays_.emplace(resourceKey, std::move(array));
  return true;
}

auto ResourceManager::unloadTextureArray(ResourceKey resourceKey) -> bool {
  DEBUG_ASSERT(textureArrays_.contains(resourceKey),
               std::format("Texture array '{}' must be loaded to unload it",
                           resourceKey));

  if (!textureArrays_.erase(resourceKey)) {
    logzy::warn("Couldn't unload texture array {}", resourceKey);
    return false;
  }
  logzy::trace("Unloaded texture array {}", resourceKey);
  return true;
}

auto ResourceManager::getTextureArray(ResourceKey resourceKey)
    -> const TextureArray & {
  DEBUG_ASSERT(
      textureArrays_.contains(resourceKey),
      std::format("Texture '{}' must be loaded to use it", resourceKey));
  return textureArrays_.at(resourceKey);
}

auto ResourceManager::verifyPath(std::filesystem::path path) noexcept -> bool {
  if (!std::filesystem::exists(path)) {
    DEBUG_ONLY(logzy::warn("Path: '{}' doesn't exist", path.string()));
    return false;
  }

  return true;
}
