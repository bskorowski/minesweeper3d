#pragma once

#include <string>
#include <string_view>

#define ERR(kind, ...) Error{Error::Kind::kind, __VA_ARGS__};

class Error {
public:
  enum class Kind;

  Error(Error::Kind kind, std::string_view msg);
  ~Error() = default;
  [[nodiscard]] auto message() const noexcept -> std::string_view;

private:
  Kind kind_;
  std::string message_;
};

enum class Error::Kind {
  GraphicsError, // Graphics API
  WindowError,   // Window
  RuntimeError   // Any runtime error
};
