#pragma once

#include "debug.hpp"
#include <string>
#include <string_view>

#define ERR(kind, ...) Error{Error::Kind::kind, __VA_ARGS__};

class Error {
public:
  enum class Kind;

  constexpr Error(Error::Kind kind, std::string_view msg);
  ~Error() = default;
  [[nodiscard]] constexpr auto message() const noexcept -> std::string_view;
  [[nodiscard]] constexpr auto kind() const noexcept -> Kind;

private:
  Kind kind_;
  std::string message_;
};

enum class Error::Kind {
  GraphicsError, // Graphics API
  WindowError,   // Window
  RuntimeError   // Any runtime error
};

constexpr Error::Error(Error::Kind kind, std::string_view msg)
    : kind_{kind}, message_{msg} {
  ASSERT(!message_.empty(), "Cannot create an error with empty message");
}

[[nodiscard]] constexpr auto Error::message() const noexcept
    -> std::string_view {
  ASSERT(!message_.empty(), "Error should have a message");
  return std::string_view{message_};
}

[[nodiscard]] constexpr auto Error::kind() const noexcept -> Kind {
  return kind_;
}
