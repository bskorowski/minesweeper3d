#include "error.hpp"
#include "debug_utils.hpp"

Error::Error(Error::Kind kind, std::string_view msg)
    : kind_{kind}, message_{msg} {
  DEBUG_ASSERT(!message_.empty(), "Cannot create an error with empty message");
}

auto Error::message() const noexcept -> std::string_view {

  DEBUG_ASSERT(!message_.empty(), "Error should have a message");
}
