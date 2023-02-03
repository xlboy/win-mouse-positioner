#include <iostream>
#include <filesystem>
#include <gtest/gtest.h>
#include <stdexcept>
#include "../index.hpp"
#include "../../utils/vk-codes.h"
#include "../../utils/g-test.hpp"

TEST(HotkeyHandle, 1) {
  using namespace std;

  enum class E {
    A,
    B,
    C
  };
  Hotkey::Manager<E> h;

  bool isTriggered = false;

  h.add(
    E::A,
    Hotkey::Entity{
      {
        VK_F1,
        { VK_CONTROL, VK_SHIFT },
      },
      [&](bool isDown) {
        isTriggered = true;
      } }
  );

  h.handle(VK_F1, true);
  EXPECT_EQ(isTriggered, false);

  isTriggered = false;
  GTestUtils::Keyboard::downKey(VK_CONTROL);
  GTestUtils::Keyboard::downKey(VK_SHIFT);
  h.handle(VK_F1, true);
  EXPECT_EQ(isTriggered, true);
  GTestUtils::Keyboard::upKey(VK_CONTROL);
  GTestUtils::Keyboard::upKey(VK_SHIFT);

  isTriggered = false;
  GTestUtils::Keyboard::downKey(VK_CONTROL);
  h.handle(VK_F1, true);
  EXPECT_EQ(isTriggered, false);
  GTestUtils::Keyboard::upKey(VK_CONTROL);
}