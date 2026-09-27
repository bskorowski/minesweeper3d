#include "input.hpp"
#include "math/trig.hpp"
#include "tasty/tasty.hpp"
#include <cmath>

int main() {
  bool allPassed = true;

  tasty::TestRunner powSuite("Powering");

  powSuite.registerTest(
      []() {
        float result1 = math::internal::pow(2.0f, 1);
        float result2 = math::internal::pow(2.0f, 2);
        float result3 = math::internal::pow(2.0f, 3);
        float result4 = math::internal::pow(2.0f, 4);
        tasty::expectEqual(2.0f, result1);
        tasty::expectEqual(4.0f, result2);
        tasty::expectEqual(8.0f, result3);
        tasty::expectEqual(16.0f, result4);
      },
      "Simple power of 2");

  powSuite.registerTest(
      []() {
        for (float x = 1.0f; x < 10.0f; x += 0.5f) {
          for (size_t i = 0; i < 32; ++i) {
            tasty::expectEqual(std::round(math::internal::pow(x, i)),
                               std::round(std::powf(x, i)));
          }
        }
      },
      "power tests");

  tasty::TestRunner trigSuite("Cosinus");

  trigSuite.registerTest(
      []() {
        for (float x = -10.0f; x < 10.0f; x += 1.0f) {
          float got = math::internal::cos_taylor(x);
          float exp = std::cos(x);
          tasty::expectEqual(got, exp);
        }
        //
      },
      "math::cos_taylor tests");
  trigSuite.registerTest(
      []() {
        for (float x = -10.0f; x < 10.0f; x += 1.0f) {
          float got = math::cos(x);
          float exp = std::cos(x);
          tasty::expectEqual(got, exp);
        }
        //
      },
      "math::cos tests");
  trigSuite.registerTest(
      []() {
        for (float x = -10.0f; x < 10.0f; x += 1.0f) {
          // 5-digit precision is enough for it I guess
          // We may lose the precision compared ot math::cos, because we shift
          // by 3/2pi, the impl is the same.
          float got = std ::round(math::sin(x) * 100'000) / 100'000;
          float exp = std::round(std::sin(x) * 100'000) / 100'000;
          tasty::expectEqual(got, exp);
        }
        //
      },
      "math::sin tests (up to 5 decimal digits)");

  allPassed &= powSuite.runAll();
  allPassed &= trigSuite.runAll();

  return allPassed ? 0 : 1;
}
