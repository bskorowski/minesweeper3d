#include "resource_manager.hpp"
#include "debug.hpp"
#include "imgui.h"
#include "render/texture.hpp"
#include "utility/cast.hpp"
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

  ASSERT(width > 0, "Actual width: {}", width);
  ASSERT(height > 0, "Actual width: {}", height);
  ASSERT(channels == desiredChannels, "channels={} desired={}", channels,
         desiredChannels);

  Texture texture(data, width, height, params);
  texture.generateMipMaps();

  ResourceManager::textures_.emplace(resourceKey, std::move(texture));
  //
  stbi_image_free(data);
  return true;
}

auto ResourceManager::unloadTexture(ResourceKey resourceKey) -> bool {
  ASSERT(textures_.contains(resourceKey),
         "Texture '{}' must be loaded to unload it", resourceKey);

  if (!textures_.erase(resourceKey)) {
    logzy::warn("Couldn't unload texture {}", resourceKey);
    return false;
  }
  logzy::trace("Unloaded texture {}", resourceKey);
  return true;
}

auto ResourceManager::getTexture(ResourceKey resourceKey) -> const Texture & {
  ASSERT(textures_.contains(resourceKey),
         "Texture '{}' must be loaded to use it", resourceKey);
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
  ASSERT(paths.size() > 0);

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

  ASSERT(data != nullptr);
  ASSERT(referenceWidth > 0, "Actual width: {}", referenceWidth);
  ASSERT(referenceHeight > 0, "Actual width: {}", referenceHeight);
  ASSERT(referenceChannels == desiredChannels, "channels={} desired={}",
         referenceChannels, desiredChannels);

  ASSERT(paths.size() < std::numeric_limits<int>::max(),
         "Ensure safe cast to int");
  TextureArray array(referenceWidth, referenceHeight,
                     cast<int>(paths.size() + 1), params);
  array.bind();

  stbi_set_flip_vertically_on_load(true);

  array.addTexture(data, layer++);
  stbi_image_free(data);

  for (std::string_view pathPart : paths) {
    currentPath = basePath / pathPart;
    logzy::info("Loading texture at: '{}'", currentPath);
    if (!verifyPath(currentPath)) {
      logzy::critical("Texture not found: {}, couldn't load layer: {}",
                      currentPath.string().c_str(), layer++);
      return false;
    }

    int width = -1;
    int height = -1;
    int channels = -1;

    data = stbi_load(currentPath.string().c_str(), &width, &height, &channels,
                     desiredChannels);

    ASSERT(data != nullptr);
    ASSERT(referenceWidth == width, "{} == {}", referenceWidth, width);
    ASSERT(referenceHeight == height, "{} == {}", referenceHeight, height);
    ASSERT(referenceChannels == channels, "{} == {}", referenceChannels,
           channels);

    array.addTexture(data, layer++);
    stbi_image_free(data);
  }

  ResourceManager::textureArrays_.emplace(resourceKey, std::move(array));
  return true;
}

auto ResourceManager::unloadTextureArray(ResourceKey resourceKey) -> bool {
  ASSERT(textureArrays_.contains(resourceKey),
         "Texture array '{}' must be loaded to unload it", resourceKey);

  if (!textureArrays_.erase(resourceKey)) {
    logzy::warn("Couldn't unload texture array {}", resourceKey);
    return false;
  }
  logzy::trace("Unloaded texture array {}", resourceKey);
  return true;
}

auto ResourceManager::getTextureArray(ResourceKey resourceKey)
    -> const TextureArray & {
  ASSERT(textureArrays_.contains(resourceKey),
         "Texture '{}' must be loaded to use it", resourceKey);
  return textureArrays_.at(resourceKey);
}

auto ResourceManager::verifyPath(std::filesystem::path path) noexcept -> bool {
  if (!std::filesystem::exists(path)) {
    DEBUG_ONLY(logzy::warn("Path: '{}' doesn't exist", path.string()));
    return false;
  }

  return true;
}

auto ResourceManager::loadFont(ResourceKey resourceKey,
                               std::string_view fontPath,
                               std::filesystem::path basePath) -> bool {
  ASSERT(resourceKey == ResourceKey::FontRegular,
         "Are you sure the {} is really a font?", resourceKey);

  std::filesystem::path assetPath = basePath / fontPath;
  logzy::debug("Loading font at: {}", assetPath);
  if (!verifyPath(assetPath)) {
    logzy::warn("Couldn't locate font at '{}'", assetPath);
    return false;
  }

  if (fonts_.contains(resourceKey)) {
    logzy::debug("Font already loaded");
    return true;
  }

  Font font;
  ImGuiIO &io = ImGui::GetIO();
  font.fontData =
      io.Fonts->AddFontFromFileTTF(assetPath.string().c_str(), 12.0f);
  if (!font.fontData) {
    logzy::error("Couldn't load font '{}'", assetPath);
  }
  io.Fonts->Build();

  fonts_.emplace(resourceKey, std::move(font));
  return true;
}

auto ResourceManager::getFont(ResourceKey resourceKey) -> const Font & {
  ASSERT(resourceKey == ResourceKey::FontRegular,
         "are you sure '{}' is really a font?", resourceKey);
  ASSERT(fonts_.contains(resourceKey), "Font '{}' must be loaded to use it",
         resourceKey);
  return fonts_.at(resourceKey);
}

auto ResourceManager::unloadFont([[maybe_unused]] ResourceKey resourceKey)
    -> bool {
  logzy::warn("Unloading fonts not yet implemented as fonts are used just in "
              "ImGui and we cannot unload a single font.");
  return false;
}
