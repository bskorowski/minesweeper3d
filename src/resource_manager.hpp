#pragma once
#include "debug_utils.hpp"
#include "render/texture.hpp"
#include <filesystem>
#include <span>
#include <string_view>
#include <unordered_map>

struct ResourceManager {

  // TODO :: Split Resource key for each resource type
  // TODO :: refactor raw references to shared pointers, as unloading may invoke
  // use-after-free

  enum class ResourceKey { TileTextureArray };
  inline static std::filesystem::path BASE_ASSET_PATH{"assets"};

  /**
   * @brief Loads texture into internal buffer
   * @param assetPath path to the asset relative to {basePath}
   * @param basePath base path of the asset, lower part of {assetPath}
   * @return true on success and False on failure
   */
  static auto loadTexture(ResourceKey resourceKey, std::string_view assetPath,
                          TextureParams params = {},
                          std::filesystem::path basePath = BASE_ASSET_PATH)
      -> bool;

  static auto unloadTexture(ResourceKey resourceKey) -> bool;

  [[nodiscard]] static auto getTexture(ResourceKey resourceKey)
      -> const Texture &;

  static auto loadTextureArray(ResourceKey resourceKey,
                               std::span<const std::string_view> paths,
                               TextureParams params = {},
                               std::filesystem::path basePath = BASE_ASSET_PATH)
      -> bool;

  static auto unloadTextureArray(ResourceKey resourceKey) -> bool;

  static auto getTextureArray(ResourceKey resourceKey) -> const TextureArray &;

private:
  static auto verifyPath(std::filesystem::path path) noexcept -> bool;

  inline static std::unordered_map<ResourceKey, Texture> textures_;
  inline static std::unordered_map<ResourceKey, TextureArray> textureArrays_;
};

template <> struct std::formatter<ResourceManager::ResourceKey> {
  template <class ParseContext>
  constexpr ParseContext::iterator parse(ParseContext &ctx) {
    return ctx.begin();
  }

  template <class FmtContext>
  FmtContext::iterator format(const ResourceManager::ResourceKey key,
                              FmtContext &ctx) const {
    using Key = ResourceManager::ResourceKey;
    static std::unordered_map<Key, std::string_view> mappings{
        {Key::TileTextureArray, "TileTextureArray"}};
    DEBUG_ASSERT(mappings.contains(key),
                 "No string representation defined for "
                 "ResourceManager::ResourceKey with int value = {}",
                 std::to_underlying(key));
    return std::format_to(ctx.out(), "{}", mappings.at(key));
  }
};
