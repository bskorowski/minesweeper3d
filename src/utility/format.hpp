#pragma once

#include "utility/static_string.hpp"
#include <logzy/logzy.hpp>
#include <tuple>
#include <typeinfo>
#include <utility>

[[nodiscard]] consteval size_t
countPlaceholders(std::string_view fmt) noexcept {
  std::size_t count{0uz};
  for (std::size_t i = 0; i < fmt.size() - 1; ++i) {
    if (fmt.at(i) == '{' && fmt.at(i + 1) == '}') {
      ++count;
    }
  }

  return count;
}

template <std::size_t N>
[[nodiscard]] constexpr auto to_static_string(const char (&str)[N]) {
  return static_string<N - 1>(str);
}

[[nodiscard]] constexpr auto to_static_string(char c) {
  static_string<1> s;
  s.data[0] = c;
  return s;
}

template <typename T>
[[nodiscard]] constexpr auto to_static_string([[maybe_unused]] T c) {
  static_assert(false, "Type doesn't have to_static_string defined.");
  return static_string<1>("x");
}

template <std::size_t OutSize, std::size_t... Indices>
constexpr void writeArg(static_string<OutSize> &out, std::size_t &writeIndex,
                        const auto &formattedArgs, std::size_t argIndex) {

  auto writeOne = [&]<std::size_t Index>() {
    if (argIndex == Index) {
      out.write(std::get<Index>(formattedArgs).view(), writeIndex);
      writeIndex += std::get<Index>(formattedArgs).size();
    }
  };

  (writeOne.template operator()<Indices>(), ...);
}

template <static_string Fmt, std::size_t OutSize, std::size_t... Indices>
constexpr auto formatImpl(const auto &formattedArgs,
                          std::index_sequence<Indices...>)
    -> static_string<OutSize> {
  static_string<OutSize> out;

  std::size_t writeIndex{0uz};
  std::size_t argIndex{0uz};

  for (std::size_t j{0uz}; j < Fmt.size(); ++j) {
    if ((j + 1) < Fmt.size() && Fmt.data[j] == '{' && Fmt.data[j + 1] == '}') {
      writeArg<OutSize, Indices...>(out, writeIndex, formattedArgs, argIndex++);
      ++j; // skip the second brace
      continue;
    }
    out.data[writeIndex++] = Fmt.data[j];
  }

  return out;
}

template <static_string Fmt, typename... Args>
constexpr auto constexprFormat(const Args &...args) {
  constexpr std::size_t placeholders = countPlaceholders(Fmt.view());

  auto formattedArgs = std::make_tuple(to_static_string(args)...);

  constexpr std::size_t argsTotalLength = std::apply(
      [](const auto &...str) { return (str.size() + ... + 0); }, formattedArgs);

  constexpr std::size_t finalStringSize =
      Fmt.size() + argsTotalLength - placeholders * 2;

  return formatImpl<Fmt, finalStringSize>(formattedArgs,
                                          std::index_sequence_for<Args...>{});
}
