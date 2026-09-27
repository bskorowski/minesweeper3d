#include "tasty/tasty.hpp"
#include "utility/format.hpp"
#include "utility/static_string.hpp"
#include <tasty/runners.hpp>

bool countPlaceholders_Test() {
  tasty::TestRunner runner("countPlaceholders");

  runner.registerTest([]() {
    constexpr auto x = countPlaceholders("hello{}{}");
    tasty::expectEqual(x, 2);
    constexpr auto y = countPlaceholders("hello");
    tasty::expectEqual(y, 0);
    constexpr auto z = countPlaceholders("{hello}");
    tasty::expectEqual(z, 0);
  });

  return runner.runAll();
}

bool writeArg_Test() {
  tasty::TestRunner runner("writeArg");

  runner.registerTest([]() {
    for (size_t i = 0; i < 17; ++i) {
      size_t writeIdx = i;
      static_string<22> str{"1234567890123456789012"};
      writeArg<22, 0>(str, writeIdx, std::make_tuple(to_static_string("hello")),
                      0);

      std::string expected = "1234567890123456789012";
      expected.replace_with_range(std::next(expected.begin(), i),
                                  std::next(expected.begin(), i + 5),
                                  std::string_view("hello"));

      tasty::expectEqual(str.view(), expected);
    }
  });

  return runner.runAll();
}

bool constexprFormat_Test() {
  tasty::TestRunner runner("constexprFormat");

  runner.registerTest([]() {
    constexpr auto x = constexprFormat<"{}Hello {}{}{} g">("x", "y", "z", "w");
    tasty::expectEqual(x.view(), "xHello yzw g");
  });

  return runner.runAll();
}

int main() {
  bool allPassed = true;

  allPassed &= countPlaceholders_Test();
  allPassed &= writeArg_Test();
  allPassed &= constexprFormat_Test();

  return allPassed ? 0 : 1;
}
