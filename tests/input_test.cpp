#include "input.hpp"
#include "tasty/tasty.hpp"

int main() {
  bool allPassed = true;

  {
    tasty::TestRunner suite("getState transitions");

    suite.registerTest(
        []() {
          tasty::expectEqual(KeyState::Pressed, getState(KeyState::Idle, true));
        },
        "Idle + down -> Pressed");

    suite.registerTest(
        []() {
          tasty::expectEqual(KeyState::Pressed,
                             getState(KeyState::Released, true));
        },
        "Released + down -> Pressed");

    suite.registerTest(
        []() {
          tasty::expectEqual(KeyState::Held, getState(KeyState::Pressed, true));
        },
        "Pressed + down -> Held");

    suite.registerTest(
        []() {
          tasty::expectEqual(KeyState::Held, getState(KeyState::Held, true));
        },
        "Held + down -> Held");

    suite.registerTest(
        []() {
          tasty::expectEqual(KeyState::Idle, getState(KeyState::Idle, false));
        },
        "Idle + up -> Idle");

    suite.registerTest(
        []() {
          tasty::expectEqual(KeyState::Idle,
                             getState(KeyState::Released, false));
        },
        "Released + up -> Idle");

    suite.registerTest(
        []() {
          tasty::expectEqual(KeyState::Released,
                             getState(KeyState::Pressed, false));
        },
        "Pressed + up -> Released");

    suite.registerTest(
        []() {
          tasty::expectEqual(KeyState::Released,
                             getState(KeyState::Held, false));
        },
        "Held + up -> Released");

    allPassed &= suite.runAll();
  }

  {
    tasty::TestRunner suite("getState full round-trip sequence");

    suite.registerTest(
        []() {
          KeyState s = KeyState::Idle;

          s = getState(s, true);
          tasty::expectEqual(KeyState::Pressed, s,
                             std::source_location::current());

          s = getState(s, true);
          tasty::expectEqual(KeyState::Held, s,
                             std::source_location::current());

          s = getState(s, true);
          tasty::expectEqual(KeyState::Held, s,
                             std::source_location::current());

          s = getState(s, false);
          tasty::expectEqual(KeyState::Released, s,
                             std::source_location::current());

          s = getState(s, false);
          tasty::expectEqual(KeyState::Idle, s,
                             std::source_location::current());
        },
        "Idle -> Pressed -> Held -> Held -> Released -> Idle");

    allPassed &= suite.runAll();
  }

  return allPassed ? 0 : 1;
}
