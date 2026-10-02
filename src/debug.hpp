#pragma once

#if defined(ENABLE_DEBUG_UTILS)

#include <cstdlib>
#include <format>
#include <iostream>
#include <source_location>

// As of 26.09.2026 clang doesn't support stacktrace, so we keep it optional
#if defined(__cpp_lib_stacktrace)
#include <stacktrace>
#endif

namespace debugutils {
constexpr bool DEBUG = false; // NOLINT
} // namespace debugutils

#define DEBUG_ONLY(...) __VA_ARGS__

[[noreturn]] inline void
handleAssertFail(std::string_view expr, std::string_view message,
                 const std::source_location &sourceLoc) {
  std::cerr << "\nAssertion failed\n";
  std::cerr << "Expression: " << expr << '\n';
  if (!message.empty()) {
    std::cerr << "Message: " << message << '\n';
  }
  std::cerr << "File: " << sourceLoc.file_name() << '\n';
  std::cerr << "Line: " << sourceLoc.line() << '\n';

// As of 26.09.2026 clang doesn't support stacktrace, so we keep it optional
#if defined(__cpp_lib_stacktrace)
  std::cerr << "Stacktrace:\n"
            << std::to_string(std::stacktrace::current()) << '\n';
#endif
  std::abort();
}

#define GET_FIRST(First, ...) First // NOLINT

// "Generic" assert, with no default arguments. Used when there is a need to
// explicitly provide the arguments instead of using the default ones
#define ASSERT_LOC(expr, sourceLoc, ...)                                       \
  (static_cast<bool>(expr)                                                     \
       ? void(0)                                                               \
       : handleAssertFail(                                                     \
             #expr, GET_FIRST(__VA_OPT__(std::format(__VA_ARGS__), ) ""),      \
             sourceLoc));

// Standard assertion for general purposes.
#define ASSERT(expr, ...)                                                      \
  ASSERT_LOC(expr, std::source_location::current(), __VA_ARGS__)

#else

namespace debugutils {
constexpr bool DEBUG = true; // NOLINT
} // namespace debugutils

#define DEBUG_ONLY(...)
#define ASSERT_LOC(...)
#define ASSERT(...)

#endif
